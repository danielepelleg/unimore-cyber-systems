<br />    
<p align="center">
<h1 align="center">Django My Blog</h1>
<p align="center">
    Tutorial Project for learning Django.
</p>

## Table of Contents

- [Table of Contents](#table-of-contents)
- [Getting Started](#getting-started)

## Getting Started
1. Create Python Enviroment by running:
```bash
$> python -m venv env
```
Use `python3` instead of `python` command if you use a UNIX system.

Create then a `.env` where the following keys must be specified:
- `DATABASE_NAME`
- `DATABASE_USER`
- `DATABASE_PASSWORD`

2. Install the python dependencies by running:
```bash
$> pip install -r requirements.txt
```

4. Create Superuser and populate the database:
```bash
$> python manage.py createsuperuser
```
Once the superuser has been created, run the server: 
```bash
$> python manage.py runserver
```
Navigate to: [localhost/admin](http://127.0.0.1:8000/admin) and login to start populate the database.

It is possible to populate the database even by not creating a superuser, by going to [localhost/insertAuthor](http://127.0.0.1:8000/insertAuthor) to insert an Author Object, [localhost/insertArticle](http://127.0.0.1:8000/insertArticle)  to insert an Article.


5. Navigate to:
- `localhost:8000/<int:author>/posts/<int:year>/<int:month>` 
- `localhost:8000/<int:author>/posts/<int:year>/<int:month>/<int:post_id>` 






