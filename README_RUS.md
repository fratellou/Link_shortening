# Link_shortening

Требуется реализовать сервис для сокращения ссылок. Сервис должен принимать запросы по http протоколу. Сервис должен иметь две конечные точки: 
1. Сокращение ссылки URL: / Метод: POST Запрос содержит ссылку которую нужно сократить. Ответ содержит сокращенную ссылку. 
2. Переход по сокращённой ссылке URL: / Метод: GET Ответ должен перенаправлять пользователя на исходную страницу (см. перенаправления) Для хранения сокращенных ссылок используется СУБД из репозитория https://github.com/fratellou/NoSQL-database. Реализовать клиента для СУБД на том языке, на которым вы пишите сервис сокращения ссылок.

## Оглавление

1. [Теория](#теория)
2. [Код](#код)

## Теория

HTTP протокол: HTTP (Hypertext Transfer Protocol) является протоколом передачи данных в Интернете. Он используется для передачи запросов от клиента к серверу и ответов от сервера к клиенту. Запросы содержат методы (GET, POST, PUT, DELETE и т. д.), заголовки и, в случае POST и PUT запросов, тело сообщения. Ответы содержат статус коды, заголовки и тело сообщения.

СУБД (Система Управления Базами Данных): Это программное обеспечение, которое используется для создания и управления базами данных. СУБД NoSQL, упомянутая в задании, представляет собой базу данных, которая не использует традиционную реляционную модель данных. Вместо этого она использует другие модели, такие как ключ-значение и т. д. В данном случае, предполагается использование NoSQL базы данных для хранения сокращенных ссылок.

Клиент-серверная архитектура: сервис сокращения ссылок работает по принципу клиент-серверной архитектуры. Сервер принимает HTTP запросы от клиентов, обрабатывает их и возвращает соответствующие HTTP ответы.

Компиляции программы через Makefile:
> make

Далее необходимо запустить сервер:
> ./server

Следующий шаг - запуск сервиса по сокращению ссылок:
> python3 shortener.py

И наконец - запуск клиента:
> python3 client.py

## Код

### database.c

`main()` - основная функция сервера:
- Инициализирует мьютекс для обеспечения безопасности доступа к общим ресурсам;
- Создает сокет для сервера с использованием функции `socket()`. Если создание сокета не удалось, программа завершается с ошибкой;
- Настраивает адрес сервера с помощью структуры `server_address`, указывая семейство адресов AF_INET, IP-адрес INADDR_ANY и порт, который будет слушать сервер;
- Привязывает сокет к адресу сервера с помощью функции `bind()`. Если привязка не удалась, программа завершается с ошибкой;
- Начинает прослушивать входящие соединения с помощью функции `listen()`. Если прослушивание не удалось, программа завершается с ошибкой;
- В бесконечном цикле принимает входящие соединения от клиентов с помощью функции `accept()`. При каждом подключении клиента создает новый поток для обработки запросов клиента в функции `handle_client()`;
- По завершении работы закрывает сокет сервера и уничтожает мьютекс.

`handle_client()` - функция для обработки запроса клиента. Принимает указатель на аргумент `arg`, который является сокетом клиента:
- Получает запрос от клиента через сокет и разбирает его;
- Вызывает функцию `request()` для обработки запроса;
- Отправляет ответ клиенту через сокет и закрывает соединение.

`request()` - осуществляет маршрутизацию запросов к соответствующим функциям обработки. Принимает имя файла базы данных, запрос от клиента и указатель на переменную для ответа:
- Определяет тип запроса (в данном случае только HSET, HDEL, HGET) и вызывает соответствующую функцию для обработки запроса;
- В случае некорректного запроса или ошибки вызывает вывод ошибки.

### struct_reader.c

`read_struct_from_file()` - функция предназначена для чтения данных определенной структуры из файла базы данных. Принимает `line` - массив указателей на строки, в который будут сохранены данные структуры, `filename` - имя файла базы данных, из которого будут считываться данные, `*isnt_empty` - указатель на переменную, которая будет установлена в 1, если данные структуры в файле найдены, `struct_name` - имя структуры, данные которой необходимо прочитать, `*size` - указатель на переменную, в которую будет записано количество прочитанных строк, `struct_type` - тип структуры, которую необходимо прочитать:
- Функция пытается открыть файл базы данных для чтения;
- Если файл не найден, функция пытается создать его для последующего использования;
- Далее файл закрывается и снова открывается для чтения;
- Если файл успешно открыт, функция начинает построчное чтение файла;
- Каждая строка считывается и разбивается на отдельные слова с помощью функции `strtok()`;
- При нахождении строки, соответствующей указанной структуре, данные этой строки добавляются в массив `line`;
- После завершения чтения файла функция закрывает его;
- В случае ошибки при открытии файла или чтении данных выводится соответствующее сообщение об ошибке.

### hash.c

`hash()` - выполняет операции хэширования:
- Выделяет память для массива строк `line`, который будет содержать строки из файла;
- Создает хэш-таблицу с помощью функции `createHashTable()`;
- Считывает структуры из файла с помощью функции `read_struct_from_file()`;
- Если файл не пустой (`isnt_empty == 1`), проходится по строкам, извлекает ключ и значение, и добавляет их в хэш-таблицу с помощью функции `HSET()`;
- Выполняет команды, связанные с хэш-таблицей, используя функцию `hash_commands()`;
- Записывает хэш-таблицу обратно в файл с помощью функции `write_hash()`;
- Освобождает память, выделенную для массива `line` и хэш-таблицы.

`hash_commands()` - обрабатывает команды, связанные с хэш-таблицей:
- Проверяет тип команды (HSET, HDEL, HGET) и вызывает соответствующую функцию для выполнения этой команды;
- Копирует результат выполнения команды в переменную `req`.

`createHashTable()` создает новую хэш-таблицу заданного размера:
- Выделяет память под структуру `HashTable`;
- Выделяет память под массив указателей на узлы хэш-таблицы и инициализирует их нулями.

`hash_calc()` вычисляет хэш-код для заданного ключа:
- Считает сумму ASCII-кодов символов ключа;
- Возвращает остаток от деления этой суммы на размер хэш-таблицы.

`HSET()`, `HDEL()`, `HGET()` выполняют соответствующие операции (добавление, удаление, получение элемента) в хэш-таблице. Они осуществляют поиск узла с заданным ключом и выполняют соответствующие действия:
- HSET: Если элемент с данным ключом уже существует, обновляет его значение, иначе добавляет новый элемент в конец списка;
- HDEL: Удаляет элемент с заданным ключом из хэш-таблицы;
- HGET: Извлекает значение элемента с заданным ключом.

`write_hash()` - записывает содержимое хэш-таблицы в файл:
- Открывает временный файл для записи;
- Считывает содержимое исходного файла и записывает его во временный файл, исключая структуру данных, которая уже была обновлена;
- Добавляет в конец временного файла обновленное содержимое хэш-таблицы;
- Заменяет исходный файл временным и закрывает файлы.

`destroyHashTable()` - освобождает память, занятую хэш-таблицей и ее узлами:
- Проходится по каждому элементу массива указателей на узлы и освобождает память, выделенную для узлов;
- Освобождает память, выделенную для массива указателей на узлы и самой структуры `HashTable`.

### shortener.py

`save_to_database()` - сохраняет сокращенный ключ и длинную URL-ссылку в базе данных:
- Создает сокет для подключения к серверу базы данных;
- Устанавливает соединение с сервером базы данных, указанным в переменных `DB_HOST` и `DB_PORT`;
- Подготавливает команду для сохранения данных в базе данных в формате `HSET hash {short_key} {long_url}`;
- Отправляет команду на сервер базы данных;
- Закрывает соединение с сервером.

`get_from_database()` извлекает длинную URL-ссылку из базы данных по сокращенному ключу:
- Создает сокет для подключения к серверу базы данных;
- Устанавливает соединение с сервером базы данных, указанным в переменных `DB_HOST` и `DB_PORT`;
- Подготавливает команду для получения данных из базы данных в формате `HGET hash {short_key}`;
- Отправляет команду на сервер базы данных и получает ответ;
- Закрывает соединение с сервером;
- Возвращает полученный ответ (длинную URL-ссылку).

`shorten_url()` - является обработчиком POST-запросов к корневому URL-адресу:
- Получает длинную URL-ссылку из тела POST-запроса;
- Генерирует уникальный сокращенный ключ, используя хэш SHA-1 от длинной URL-ссылки;
- Сохраняет сокращенный ключ и длинную URL-ссылку в базе данных с помощью функции `save_to_database()`;
- Возвращает сокращенный ключ в качестве ответа.

`redirect_to_original()` - является обработчиком GET-запросов с сокращенным ключом в URL-адресе:
- Получает длинную URL-ссылку из базы данных по сокращенному ключу с помощью функции `get_from_database()`;
- Если длинная URL-ссылка найдена, выполняется редирект на эту ссылку с помощью функции `redirect()`;
- Если длинная URL-ссылка не найдена, возвращается сообщение об ошибке "URL not found".

### client.py

`shorten_url()` - отправляет POST-запрос на сервис сокращения URL:
- Задает URL сервиса сокращения;
- Отправляет POST-запрос с передачей оригинального URL в качестве данных;
- Если получен ответ с кодом 200, возвращает текст ответа (сокращенный URL), иначе возвращает строку "Error".

`redirect_to_original()` - выполняет редирект на оригинальный URL по сокращенному ключу:
- Формирует URL сервиса сокращения с передачей сокращенного ключа;
- Отправляет GET-запрос для редиректа на оригинальный URL с отключением автоматического редиректа;
- Если получен ответ с кодом 302 (редирект), извлекает абсолютный URL из заголовка "Location" и выводит его на экран, иначе выводит текст ответа.

В блоке if `__name__ == '__main__'`: 
- Ввод оригинального URL, сокращение URL, вывод на экран сокращенного URL;
- Формирование полного сокращенного URL с использованием базового адреса сервиса сокращения;
- Вызов функции `redirect_to_original()` для редиректа на оригинальный URL;
- Если происходит ошибка во время сокращения URL, выводится сообщение "Error".

>
> fratellou, 2023