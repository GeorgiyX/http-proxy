## Сборка

```bash
sudo docker build -t http_proxy . 
```

## Запуск

```bash
sudo docker run -p 80:80 -t -i http_proxy
```

## Тест

```bash
curl -x http://127.0.0.1:80 http://mail.ru
```

