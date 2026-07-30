#!/bin/sh
set -eu

if [ -z "${KFC_POSTGRES_USER:-}" ] || [ -z "${KFC_POSTGRES_PASSWORD:-}" ]; then
    echo "PostgreSQL application user and password are required" >&2
    exit 1
fi

if [ "$KFC_POSTGRES_USER" = "$POSTGRES_USER" ]; then
    echo "PostgreSQL application user must differ from the bootstrap administrator" >&2
    exit 1
fi

psql \
    --username "$POSTGRES_USER" \
    --dbname "$POSTGRES_DB" \
    --set=ON_ERROR_STOP=1 \
    --set=app_database="$POSTGRES_DB" \
    --set=app_user="$KFC_POSTGRES_USER" \
    --set=app_password="$KFC_POSTGRES_PASSWORD" <<-'SQL'
SELECT format('CREATE ROLE %I', :'app_user')
WHERE NOT EXISTS (
    SELECT 1 FROM pg_roles WHERE rolname = :'app_user'
)
\gexec

SELECT format(
    'ALTER ROLE %I WITH LOGIN PASSWORD %L NOSUPERUSER NOCREATEDB NOCREATEROLE NOINHERIT NOREPLICATION NOBYPASSRLS',
    :'app_user',
    :'app_password'
)
\gexec

SELECT format(
    'GRANT CONNECT ON DATABASE %I TO %I',
    :'app_database',
    :'app_user'
)
\gexec

SELECT format(
    'GRANT USAGE, CREATE ON SCHEMA public TO %I',
    :'app_user'
)
\gexec
SQL
