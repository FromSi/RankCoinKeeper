-- Таблица чатов телеграм
CREATE TABLE IF NOT EXISTS chats (
  chat_id INTEGER PRIMARY KEY, -- Идентификатор текущей таблицы
  name TEXT NOT NULL, -- Название чата в телеграм
  username TEXT NOT NULL, -- Username чата в телеграм
  is_active INTEGER NOT NULL, -- Активность в чате
  is_admin INTEGER NOT NULL -- Права администратора
);
