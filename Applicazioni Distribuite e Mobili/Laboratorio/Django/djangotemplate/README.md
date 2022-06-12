<br />    
<p align="center">
<h1 align="center">Django Template</h1>
<p align="center">
    Template Project for Django.
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
- `SECRET_KEY`
- `PROJECT_NAME`

1. Install the python dependencies by running:

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

Navigate to: [localhost/admin](http://127.0.0.1:8000/admin) and login to start populate the database. Create some Products to begin.

Create a new customer by going to: [localhost/accounts/registration](http://127.0.0.1:8000/accounts/registration). Once the Customer has been created, sign in using the form at [localhost/accounts/login](http://127.0.0.1:8000/accounts/login).
