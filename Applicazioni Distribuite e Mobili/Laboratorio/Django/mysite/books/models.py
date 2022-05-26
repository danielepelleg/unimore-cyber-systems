from statistics import mode
from django.db import models

# Create your models here.

TITLE_CHOICES = ( ('MR', 'Mr.'), ('MRS', 'Mrs.'), ('MS', 'Ms.') )

class Author(models.Model):
    name = models.CharField(max_length=100)
    title = models.CharField(max_length=3, choices=TITLE_CHOICES)
    nationality = models.CharField(max_length=100, blank=True)

    def __str__(self) -> str:
        return self.name

class Book(models.Model):
    name = models.CharField(max_length=100)
    authors = models.ManyToManyField(Author)

    def __str__(self) -> str:
        return self.name