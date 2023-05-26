-- Таблица чатов телеграм
CREATE TABLE IF NOT EXISTS chats (
  chat_id INTEGER PRIMARY KEY, -- Идентификатор текущей таблицы
  name TEXT DEFAULT '' NOT NULL, -- Название чата в телеграм
  username TEXT DEFAULT '' NOT NULL, -- Username чата в телеграм
  is_active INTEGER DEFAULT 0 NOT NULL, -- Активен ли сейчас в чате бот
  is_admin INTEGER DEFAULT 0 NOT NULL, -- Права администратора для бота
  member_count INTEGER DEFAULT 0 NOT NULL -- Количество человек в группе
);

-- Таблица пользователей телеграм
CREATE TABLE IF NOT EXISTS users (
  id INTEGER PRIMARY KEY, -- Идентификатор текущей таблицы
  first_name TEXT DEFAULT '' NOT NULL, -- Имя пользователя в телеграм
  username TEXT DEFAULT '' NOT NULL, -- Username пользователя в телеграм
  is_premium INTEGER DEFAULT 0 NOT NULL -- Премиум аккаунт в телеграм
);

-- Таблица чатов и пользователей телеграм (bridge chats and users)
CREATE TABLE IF NOT EXISTS chats_and_users (
  chat_id INTEGER,
  user_id INTEGER,
  is_admin INTEGER DEFAULT 0 NOT NULL, -- Является ли пользователь администратором в группе
  is_left INTEGER DEFAULT 0 NOT NULL, -- Вышел ли пользователь из группы
  is_banned INTEGER DEFAULT 0 NOT NULL, -- Забанен ли пользователь в группе
  is_active INTEGER DEFAULT 0 NOT NULL, -- Активен ли пользователь в группе
  admin_at INTEGER DEFAULT 0 NOT NULL, -- Дата последнего администрирования в группе (POSIX time)
  left_at INTEGER DEFAULT 0 NOT NULL, -- Дата последнего выхода пользователя из группы (POSIX time)
  banned_at INTEGER DEFAULT 0 NOT NULL, -- Дата последнего бана пользователя в группе (POSIX time)
  active_at INTEGER DEFAULT 0 NOT NULL, -- Дата последней активности в группе (POSIX time)
  can_send_audio_at INTEGER DEFAULT 0 NOT NULL, -- Дата окончания отправки аудио (POSIX time)
  can_send_document_at INTEGER DEFAULT 0 NOT NULL, -- Дата окончания отправки документов (POSIX time)
  can_send_photo_at INTEGER DEFAULT 0 NOT NULL, -- Дата окончания отправки фотографий (POSIX time)
  can_send_video_at INTEGER DEFAULT 0 NOT NULL, -- Дата окончания отправки видео (POSIX time)
  can_send_video_note_at INTEGER DEFAULT 0 NOT NULL, -- Дата окончания отправки видео тайм-кодов  (POSIX time)
  can_send_voice_note_at INTEGER DEFAULT 0 NOT NULL, -- Дата окончания отправки аудио тайм-кодов (POSIX time)
  can_send_poll_at INTEGER DEFAULT 0 NOT NULL, -- Дата окончания отправки викторин (POSIX time)
  can_add_web_page_preview_at INTEGER DEFAULT 0 NOT NULL, -- Дата окончания отправки веб превью по ссылке (POSIX time)
  can_send_other_message_at INTEGER DEFAULT 0 NOT NULL, -- Дата окончания отправки анимаций, стикеров и игр (POSIX time)
  
  PRIMARY KEY (chat_id, user_id)
);
