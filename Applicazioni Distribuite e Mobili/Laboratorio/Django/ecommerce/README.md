<br />    
<p align="center">
<h1 align="center">Django Ecommerce</h1>
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
- `SECRET_KEY`
  
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

Create a new customer by going to: [localhost/accounts/registration](http://127.0.0.1:8000/accounts/registration). Once the Customer has been created, sign in using the form at [localhost/accounts/login](http://127.0.0.1:8000/accounts/login). If the login success, you will be redirect to [localhost/index](http://127.0.0.1:8000/index). Here you can start by searching products and buying them.


5. Navigate to:
- `localhost:8000/accounts/registration`
- `localhost:8000/accounts/login`
- `localhost:8000/index`
- `localhost:8000/products`
- `localhost:8000/<int:product_id>`
- `localhost:8000/cart/<int:user_id>`
- `localhost:8000/cart/<int:user_id>/orders`
- `localhost:8000/cart/<int:user_id>/orders/<int:order_id>`

6. Here are the POST request path used:
- `localhost:8000/<int:product_id>/buy` &rarr; add the specified quantity of the given product to the user's cart
- `localhost:8000/cart/<int:user_id>/checkout` &rarr; order products in the cart for the user







