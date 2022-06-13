from django.db import models
from django.contrib.auth.models import User

# Create your models here.

class Author(models.Model):
    TITLE_CHOICES = ( ('MR', 'Mr.'), ('MRS', 'Mrs.'), ('MS', 'Ms.') )
    
    user = models.OneToOneField(User, on_delete=models.CASCADE)
    full_name = models.CharField(max_length=70)
    title = models.CharField(max_length=3, choices=TITLE_CHOICES)
    nationality = models.CharField(max_length=100, blank=True)

    def get_fields_values(self):
        return [(field.attname, field.value_to_string(self)) for field in Author._meta.fields]

    def __str__(self) -> str:
        return self.full_name

class Article(models.Model):
    title = models.CharField(max_length=200)
    content = models.TextField()
    pubblication_date = models.DateField(auto_now=True)
    author = models.ForeignKey(Author, on_delete=models.CASCADE)
    #author = models.ManyToManyField(Author)

    def get_fields_values(self):
        return [(field.attname, field.value_to_string(self)) for field in Article._meta.fields]
    
    def __str__(self) -> str:
        return self.title
