from django.db import models
from django.contrib.auth.models import User
from django.forms import CharField

# Create your models here.

class Skill(models.Model):
    name = models.CharField(max_length=30)
    
    def __str__(self) -> str:
        return self.name

class Profile(models.Model):
    user = models.OneToOneField(User, on_delete=models.CASCADE)
    image = models.ImageField(blank=True, upload_to='profile_url')
    country = models.CharField(blank=True, max_length=50)
    city = models.CharField(blank=True, max_length=50)
    birth_date = models.DateField(blank=True)
    #skills = models.ManyToManyField(Skill, through='HasSkill')

    def __str__(self) -> str:
        return "user " + str(self.user.username)

