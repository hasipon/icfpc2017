# icfpc2017

## Dashboard
初回のみ
```
$cd dashboard
$./setup.sh
```

立ち上げ
```
$cd dashboard
$export FLASK_APP=dashboard.py
$export FLASK_DEBUG=1
$./venv/bin/python3 -m flask run
```

### ログのアップロード
`multipart/form-data` 形式で `file=filename` を POSTする。成功したら204が返る。
`/logs/filename` からダウンロードできる。

例
```sh
echo "test" > test
curl -F 'file=@test' http://13.114.38.186/uploadlog
curl http://13.114.38.186/logs/test
```

