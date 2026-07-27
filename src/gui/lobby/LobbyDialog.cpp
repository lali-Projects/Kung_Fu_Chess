#include "LobbyDialog.hpp"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QTimer>
#include <QVBoxLayout>

#include <chrono>
#include <exception>
#include <utility>

#include "ClientApplicationController.hpp"


LobbyDialog::LobbyDialog(
    ClientApplicationController& controller,
    QWidget* parent)
    : QDialog(parent),
      m_controller(controller),
      m_serverUrl(new QLineEdit("ws://127.0.0.1:8080", this)),
      m_username(new QLineEdit(this)),
      m_password(new QLineEdit(this)),
      m_roomId(new QLineEdit(this)),
      m_status(new QLabel("Connect to a server to begin.", this)),
      m_connect(new QPushButton("Connect", this)),
      m_register(new QPushButton("Register", this)),
      m_login(new QPushButton("Login", this)),
      m_createRoom(new QPushButton("Create Room", this)),
      m_joinRoom(new QPushButton("Join Room", this)),
      m_operationTimer(new QTimer(this)),
      m_stateTimer(new QTimer(this))
{
    setWindowTitle("Kung Fu Chess Lobby");
    setMinimumWidth(420);
    m_password->setEchoMode(QLineEdit::Password);
    m_status->setWordWrap(true);

    auto* form = new QFormLayout;
    form->addRow("Server URL", m_serverUrl);
    form->addRow("Username", m_username);
    form->addRow("Password", m_password);
    form->addRow("Room ID", m_roomId);

    auto* accountButtons = new QHBoxLayout;
    accountButtons->addWidget(m_register);
    accountButtons->addWidget(m_login);

    auto* roomButtons = new QHBoxLayout;
    roomButtons->addWidget(m_createRoom);
    roomButtons->addWidget(m_joinRoom);

    auto* layout = new QVBoxLayout(this);
    layout->addLayout(form);
    layout->addWidget(m_connect);
    layout->addLayout(accountButtons);
    layout->addLayout(roomButtons);
    layout->addWidget(m_status);

    m_operationTimer->setInterval(25);
    connect(
        m_operationTimer,
        &QTimer::timeout,
        this,
        [this]()
        {
            pollOperation();
        });

    m_lastConnected = m_controller.isConnected();
    m_lastAuthenticated = m_controller.isAuthenticated();
    m_stateTimer->setInterval(100);
    connect(
        m_stateTimer,
        &QTimer::timeout,
        this,
        [this]()
        {
            refreshClientState();
        });
    m_stateTimer->start();

    connect(
        m_connect,
        &QPushButton::clicked,
        this,
        [this]()
        {
            const std::string url =
                m_serverUrl->text().trimmed().toStdString();
            auto* controller = &m_controller;
            runOperation(
                "Connect",
                false,
                [controller, url]()
                {
                    return controller->connectServer(url);
                });
        });

    connect(
        m_register,
        &QPushButton::clicked,
        this,
        [this]()
        {
            const std::string username =
                m_username->text().trimmed().toStdString();
            const std::string password =
                m_password->text().toStdString();
            auto* controller = &m_controller;
            runOperation(
                "Register",
                false,
                [controller, username, password]()
                {
                    return controller->registerUser(
                        username,
                        password);
                });
        });

    connect(
        m_login,
        &QPushButton::clicked,
        this,
        [this]()
        {
            const std::string username =
                m_username->text().trimmed().toStdString();
            const std::string password =
                m_password->text().toStdString();
            auto* controller = &m_controller;
            runOperation(
                "Login",
                false,
                [controller, username, password]()
                {
                    return controller->login(
                        username,
                        password);
                });
        });

    connect(
        m_createRoom,
        &QPushButton::clicked,
        this,
        [this]()
        {
            const std::string roomId =
                m_roomId->text().trimmed().toStdString();
            auto* controller = &m_controller;
            runOperation(
                "Create room",
                false,
                [controller, roomId]()
                {
                    return controller->createRoom(roomId);
                });
        });

    connect(
        m_joinRoom,
        &QPushButton::clicked,
        this,
        [this]()
        {
            const std::string roomId =
                m_roomId->text().trimmed().toStdString();
            auto* controller = &m_controller;
            runOperation(
                "Join room",
                true,
                [controller, roomId]()
                {
                    return controller->joinRoom(roomId);
                });
        });

    updateControls();
}


LobbyDialog::~LobbyDialog()
{
    m_operationTimer->stop();
    m_stateTimer->stop();
    if(m_pendingOperation.valid())
    {
        // The worker captures only the controller and value arguments. Waiting
        // here guarantees it cannot outlive the controller owned by main.
        m_pendingOperation.wait();
    }
}


void LobbyDialog::refreshClientState()
{
    const bool connected = m_controller.isConnected();
    const bool authenticated = m_controller.isAuthenticated();
    const bool connectionChanged =
        connected != m_lastConnected;
    const bool authenticationChanged =
        authenticated != m_lastAuthenticated;

    m_lastConnected = connected;
    m_lastAuthenticated = authenticated;

    if(!m_busy && connectionChanged)
    {
        m_status->setText(
            connected
                ? "Connected to server."
                : "Disconnected from server.");
    }
    else if(!m_busy &&
            authenticationChanged &&
            !authenticated)
    {
        m_status->setText("Authentication ended.");
    }

    if(connectionChanged || authenticationChanged)
    {
        updateControls();
    }
}


void LobbyDialog::runOperation(
    std::string action,
    bool acceptOnSuccess,
    std::function<MoveResult()> operation)
{
    if(m_busy)
    {
        return;
    }

    m_busy = true;
    m_pendingAction = std::move(action);
    m_acceptOnSuccess = acceptOnSuccess;
    m_status->setText(
        QString::fromStdString(m_pendingAction + " in progress..."));
    updateControls();

    try
    {
        m_pendingOperation = std::async(
            std::launch::async,
            [operation = std::move(operation)]() mutable
            {
                return operation();
            });
        m_operationTimer->start();
    }
    catch(const std::exception& exception)
    {
        m_busy = false;
        showResult(
            m_pendingAction.c_str(),
            {false, exception.what()});
        updateControls();
    }
}


void LobbyDialog::pollOperation()
{
    if(!m_busy || !m_pendingOperation.valid())
    {
        return;
    }
    if(m_pendingOperation.wait_for(std::chrono::milliseconds(0)) !=
       std::future_status::ready)
    {
        return;
    }

    m_operationTimer->stop();
    MoveResult result;
    try
    {
        result = m_pendingOperation.get();
    }
    catch(const std::exception& exception)
    {
        result = {false, exception.what()};
    }
    catch(...)
    {
        result = {false, "unexpected_client_error"};
    }

    const bool shouldAccept =
        m_acceptOnSuccess && result.success;
    m_busy = false;
    showResult(m_pendingAction.c_str(), result);
    updateControls();
    if(shouldAccept)
    {
        accept();
    }
}


void LobbyDialog::showResult(
    const char* action,
    const MoveResult& result)
{
    const QString state = result.success ? "succeeded" : "failed";
    m_status->setText(
        QString("%1 %2: %3")
            .arg(QString::fromLatin1(action))
            .arg(state)
            .arg(QString::fromStdString(result.reason)));
}


void LobbyDialog::updateControls()
{
    const bool connected = m_controller.isConnected();
    const bool authenticated = m_controller.isAuthenticated();

    m_serverUrl->setEnabled(!m_busy && !connected);
    m_username->setEnabled(!m_busy && !authenticated);
    m_password->setEnabled(!m_busy && !authenticated);
    m_roomId->setEnabled(!m_busy && authenticated);
    m_connect->setEnabled(!m_busy && !connected);
    m_register->setEnabled(!m_busy && connected && !authenticated);
    m_login->setEnabled(!m_busy && connected && !authenticated);
    m_createRoom->setEnabled(!m_busy && authenticated);
    m_joinRoom->setEnabled(!m_busy && authenticated);
}
