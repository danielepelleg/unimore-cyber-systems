# Django Cheatsheet

## Setup
```bash
$> pip install django
```

## Generics

### Create Project
```bash
$> django-admin startproject $PROJECT_NAME
```

### Run Server
```bash
$> django-admin runserver
```

### Create a New App
```bash
$> django-admin startapp $APP_NAME
```

### Python Interactive Shell
```bash
$> python manage.py shell
```

#### Shell Use
```bash
# import model
> from $app_name.models import Table1, Table2
> Table1.objects.all()
# create record
> from django.utils import timezone
> q = Table1(column1="Text", column2=timezone.now())
> q.save()
> q.id
> q.column_1
# list all objects of a table
> Table1.objects.all()
# get a sinfgle object
> q = Table1.objects.get(id=1)
# shortcut: pk (primary key) stays for id
> q = Table1.objects.get(pk=1)
# object function
> q.function_name()
# query filter
> Table1.objects.filter(<predicate>)
> Table1.objects.exclude(<predicate>)
# select objects linked to another table (using foreign keys)
> q = Table1.objects.get(pk=1)
> q.table2_set.all()
# create records belonging to Table2 linked to Table1 with a foreign key
> q = Table1.objects.get(pk=1)
> q.table2_set.create(column1='T1', column2=0)
> q.table2_set.create(column1='T2', column2=0)
> q.table2_set.create(column1='T3', column2=0)
# count records linked to another record in Table1
> q.table2_set.count()
# implicit joins
current_year = timezone.now().year
Table2.objects.filter(table1__pub_date__year = current_year)
# delete elements
Table2.objects.filter(Column1__startswith='T3').delete()
# or
q.table2_set.filter(choice_text__startswith='T3').delete() 
# Q objects (for complex query, equivalent of 'WHERE ... AND')
# '~' deny the statement 
> from django.db.mosdels import Q
> Table.objects.get(Q(table1__startswith=''), ~Q(table2__year='2022-01-01'))
```
Filtering Predicate use the following syntax: `<fieldName>__criterion=<value>`, for example:

- `column__startswith='Text'`
- `column__contains`
- `column__year`
- `column__month`
- `column__exact`

The `create()` function save data on database without need to save.

### Test Application
Test should be defined in `$PROJECT_NAME/$APP_NAME/tests.py`. Testing works on another database created by django named "test_$DATABASE_NAME".
```bash
$> python manage.py test $APP_NAME
```

The testing database is created by the default django user, is some problems occur we need to give this user the rights:
```bash
$> mysql
myql> grant all on test_dbdjango.* to 'userdjango'@'localhost'
```
The entire database is deleted once the tests are done.



## Administration Interface 

### Create Superuser
```bash
$> python manage.py createsuperuser
```
Specifying an username:
```bash
$> python manage.py createsuperuser --username=$USERNAME
```

### Create User
Using the shell:
```bash
> from django.contrib.auth.models import User
# (username, email, password, first_name, last_name, is_staff)
# By default a user has no permissions (final user)
> user = User.objects.create_user('username', 'email', 'password')
> user.save()
# User's Groups
> user.groups = [group_list]
> user.groups.add(group1, group2, ...)
> user.groups.remove(group1, group2, ...)
> user.groups.clear()
# User's Permissions
> user.user_permissions = [permission_list]
> user.user_permissions.add(permission1, permission2, ...)
> user.user_permissions.remove(permission1, permission2, ...)
> user.user_permissions.clear()
> user.has_perm('$APP_NAME.add_$MODEL_NAME') # -> boolean
> user.get_all_permission()
```

### Assign Permissions
```bash
> from django.contrib.auth import authenticate
> from django.contrib.contenttypes.models import ContentType
> from django.contrib.auth.models import Permission
> user = autheticate(username='', password='')
> content_type = ContentType.objects.get(model='$MODEL_NAME')
> permission1 = Permission.objects.get(content_type = content_type, codename = 'add_$MODEL_NAME')
> permission2 = Permission.objects.get(content_type = content_type, codename = 'change_$MODEL_NAME')
> permission3 = Permission.objects.get(content_type = content_type, codename = 'delete_$MODEL_NAME')
# Create Permission
> permission = Permission.objects.create(codename='code_name', name='Description Name', content_type=content_type)
```

## Change Password
```bash
$> python manage.py changepassword $USERNAME
```

### Edit Data on Database
In file `$APP_NAME/admin.py`, add the possibility to edit the table's data with the following code:
```python
from django.contrib import admin
from .models import Table1

# Register your models here.

admin.site.register(Table1)
```

## Database

### DBMS Configuration

In file _`$PROJECT_NAME/settings.py`_:
```python
DATABASES = {
    'default': {
        'ENGINE': 'django.db.backends.sqlite3',
        'NAME': 'dbdname',
        'USER': 'userdb',
        'PASSWORD': 'pwddb',
        'HOST': '', # default, localhost
        'PORT': '', # depends on db
    }
}
```
The default DB is **SQLite**, the databases which can be used are:
- SQLite
- MySQL
- PostgreSQL

### Database Synchronization / Create Tables
```bash
$> python manage.py migrate
```
This command creates the tables in the database if there are defined in _`$APP_NAME/models.py`_. The names of the tables are _`$app_name_$table_name`_.

### Activate the Model
The Model creates the tables in the database, which have to be described in _`$APP_NAME/models.py`_ using Python classes as it follows:
```python
from django.db import models

# Create your models here.

class Table1(models.Model):
    column1 = models.CharField(max_length=200)
    column2 = models.DateTimeField('date')

class Table2(models.Model):
    column1 = models.ForeignKey(Table1, on_delete=models.CASCADE)
    column2 = models.CharField(max_length=200)
    column3 = models.IntegerField(default=0)
```
The previous code creates the following tables in the database:

**Table1**
| ID | Column1 | Column2 |
|----|---------|---------|

**Table2**
| ID | Table1_ID | Column1 | Column2 |
|----|-----------|---------|---------|

In _`$PROECT_NAME/settings.py`_ is necessary to add:
```python
INSTALLED_APPS = (
    '$APP_NAME.apps.($APP_NAME)Config',
    'django.contrib.admin,
    [...]
)
```

### Create Migration
```bash
$> python manage.py makemigrations $APP_NAME
```
This command create the migration in _`$APP_NAME/migrations/000x_initial.py`_. To create the tables in the database use [the previous command](#database-synchronization--create-tables).

### Print SQL of Migration
```bash
$> python manage.py sqlmigrate $APP_NAME 000x
```

### Summary
To edit the model:
1. Change data model in _`$APP_NAME/models.py`_
2. Make Migration
   ```bash 
   $> python manage.py makemigrations $APP_NAME
   ```
3. Create Tables
   ```bash 
   $> python manage.py migrate
   ```
