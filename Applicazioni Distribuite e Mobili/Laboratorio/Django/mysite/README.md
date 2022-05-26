<br />    
<p align="center">
<h1 align="center">Django My Site</h1>
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

4. (Optional) Create Data in the Database

Open the python shell:
```bash
$> python manage.py shell
```
```bash
> from polls.models import Question, Choice
> from django.utils import timezone
# Create Record in Question Table
> new_question = Question(question_text="What's new?", pub_date=timezone.now())
> new_question.save()
# Create Choices associated to the Question
> new_question.choice_set.create(choice_text="Not Much", votes=0)
> new_question.choice_set.create(choice_text="The Sky", votes=0)
```

5. Create Superuser
```bash
$> python manage.py createsuperuser
```




