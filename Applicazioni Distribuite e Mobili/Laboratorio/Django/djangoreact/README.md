<br />    
<p align="center">
<h1 align="center">Django React Exam</h1>
<p align="center">
    Django Exam 14.06.2022.
</p>

## Table of Contents

- [Table of Contents](#table-of-contents)
- [Getting Started](#getting-started)
- [Exam](#exam)
  - [Models](#models)
  - [REST API](#rest-api)
  - [Integrate React Frontend](#integrate-react-frontend)

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
- `PROJECT_NAME`=djangoreact

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

## Exam

### Models

```python
class Product(models.Model):
    name = models.CharField(max_length=100)
    description = models.CharField(max_length=300)
    price = models.DecimalField(default=1.00, max_digits=5, decimal_places=2)

    def clean(self):
        from django.core.exceptions import ValidationError
        if  self.price <= 0:
            raise ValidationError({'price': ('Enter a value >= 0.')})
        super(Product, self).clean()

    def get_fields_values(self):
        return [(field.attname, field.value_to_string(self)) for field in Product._meta.fields]

    def __str__(self) -> str:
        return self.name
```

### REST API

```python
# ./products/serializers.py

class ProductSerializer(ModelSerializer):
    class Meta:
        model = Product
        fields = ['id', 'description']

# ./products/urls.py

urlpatterns = [
    path('', views.products_list_api, name='list_api'),
]

# ./products/views.py

@csrf_exempt
def products_list_api(request):
    if request.method == 'GET':
        products = Product.objects.all()
        serializer = ProductSerializer(products, many=True)
        return JsonResponse(serializer.data, safe=False)
```

### Integrate React Frontend

Create (or move if you have one already) your React project in the Django project root folder. To better understand the next steps, we refer to it as `frontend` folder. Your project structure at this point should be:

```bash
.
├── accounts
├── application
├── dbdjango
├── djangotemplate
├── env
├── frontend
    ├── node_modules
    ├── public
    ├── src
    ├── App.js
    └── index.js
├── manage.py
├── requirements.txt
├── static
└── templates
```

Move to your frontend folder and try to start your React project to see if it works with:

```bash
$> npm start
```

Now we need to build the package which the backend will make use of by running:

```bash
$> npm run build
```

Then we also need to remove /build from the gitignore file since the backend would need to make use of the templates and staticfiles in the build folder.

Now let's head to the backend setup and connect it to the react application we just created. Move to the root folder and navigate to `./djangotemplate/settings.py`. Here we have to edit two parameters in the configuration (using this template the lines will be already present but commented):

- Configure the template directory by pointing it towards the build folder in our frontend react directory.

```python
TEMPLATES = [
    {
        'BACKEND': 'django.template.backends.django.DjangoTemplates',
        'DIRS': [
            os.path.join(BASE_DIR, 'templates'),
            os.path.join(BASE_DIR, 'frontend', 'build')], # <- add this
        'APP_DIRS': True,
        'OPTIONS': {
            'context_processors': [
                'django.template.context_processors.debug',
                'django.template.context_processors.request',
                'django.contrib.auth.context_processors.auth',
                'django.contrib.messages.context_processors.messages',
            ],
        },
    },
]
```

- Configure the static files directory as well

```python
STATICFILES_DIRS = [
    os.path.join(BASE_DIR, 'frontend', 'build', 'static'),
    os.path.join(BASE_DIR, "static"),] # <- add this
```

Open the views.py file and create a simple function-based view that just renders the page using the React Frontend.

```python
# ./products/urls.py

urlpatterns = [
    path('', views.products_list_api, name='list_api'),
    # ...
    path('react/', views.products_list_react, name='list_react'),
]

# ./products/views.py

def products_list_react(request):
    return render(request, 'index.html')
```
