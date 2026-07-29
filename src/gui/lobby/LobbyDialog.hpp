#pragma once

#include <functional>
#include <future>
#include <string>

#include <QDialog>

#include "MoveResult.hpp"


class ClientApplicationController;
class QLabel;
class QLineEdit;
class QPushButton;
class QTimer;


class LobbyDialog : public QDialog
{
public:
    explicit LobbyDialog(
        ClientApplicationController& controller,
        QWidget* parent = nullptr);
    ~LobbyDialog() override;

private:
    void runOperation(
        std::string action,
        bool acceptOnSuccess,
        std::function<MoveResult()> operation);
    void pollOperation();
    void refreshClientState();
    void showResult(
        const char* action,
        const MoveResult& result);
    void updateControls();

private:
    ClientApplicationController& m_controller;
    QLineEdit* m_serverUrl;
    QLineEdit* m_username;
    QLineEdit* m_password;
    QLineEdit* m_roomId;
    QLabel* m_status;
    QPushButton* m_connect;
    QPushButton* m_register;
    QPushButton* m_login;
    QPushButton* m_createRoom;
    QPushButton* m_joinRoom;
    QTimer* m_operationTimer;
    QTimer* m_stateTimer;
    std::future<MoveResult> m_pendingOperation;
    std::string m_pendingAction;
    bool m_acceptOnSuccess{false};
    bool m_busy{false};
    bool m_lastConnected{false};
    bool m_lastAuthenticated{false};
};
