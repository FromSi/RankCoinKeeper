-- Таблица чатов телеграм
CREATE TABLE IF NOT EXISTS chats (
  chat_id INTEGER PRIMARY KEY, -- Идентификатор текущей таблицы
  name TEXT DEFAULT '' NOT NULL, -- Название чата в телеграм
  username TEXT DEFAULT '' NOT NULL, -- Username чата в телеграм
  is_active INTEGER DEFAULT 0 NOT NULL, -- Активность в чате
  is_admin INTEGER DEFAULT 0 NOT NULL, -- Права администратора
  member_count INTEGER DEFAULT 0 NOT NULL -- Количество человек в группе
);
