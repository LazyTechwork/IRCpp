# IRCpp

Реализация простого чата, вдохновлённая протоколом IRC, на C++.

Реализация содержит отдельный сервер с самописной базой пользователей и, возможно, сообщений (пока не вижу смысла писать
БД для сообщений).

## Команды

IRC основан на полной командной зависимости. Сообщение не будет принято клиентом или сервером, если отправлена
неправильная команда. Команды и реализация вдохновлены Twitch IRC.

Список планируемых команд и их примеры ниже:

| Макрос | Команда | Клиенту | Серверу |
|--|--|--|--|
| CMD_MESSAGE | MSG | `MSG 0xF0 Message` | `MSG Message` |
| CMD_SYSTEM | SYS | `SYS Message` | - |
| CMD_JOIN | JOIN | `JOIN nickname` | `JOIN nickname password` |
| CMD_PART | PART | `PART nickname` | `PART` |
| CMD_COLOR | COLOR | - | `COLOR 0xF0` |
| CMD_WHISPER | WHISPER | `WHISPER nickname Message` | `WHISPER nickname Message` |
| CMD_PING | PING | `PING` | `PING` |
| CMD_PONG | PING | `PONG` | `PONG` |


_Данный файл будет пополняться со временем. Проект подготовлен в качестве курсовой работы._