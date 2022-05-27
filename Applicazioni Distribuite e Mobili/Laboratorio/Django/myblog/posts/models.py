from django.db import models

# Create your models here.

class Author(models.Model):
    TITLE_CHOICES = ( ('MR', 'Mr.'), ('MRS', 'Mrs.'), ('MS', 'Ms.') )
    
    full_name = models.CharField(max_length=70)
    title = models.CharField(max_length=3, choices=TITLE_CHOICES)
    nationality = models.CharField(max_length=100, blank=True)

    def __str__(self) -> str:
        return self.full_name

class Article(models.Model):
    title = models.CharField(max_length=200)
    content = models.TextField()
    pubblication_date = models.DateField()
    author = models.ManyToManyField(Author)
    
    def __str__(self) -> str:
        return self.title
