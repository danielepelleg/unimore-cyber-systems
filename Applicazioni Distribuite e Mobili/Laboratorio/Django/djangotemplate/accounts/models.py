from django.db import models
from django.contrib.auth.models import User
from pytz import timezone

# Create your models here.

class Profile(models.Model):
    GENDERS = [('M', 'Male'), ('F', 'Female')]
    LOCATION = [('EUR', 'Europe'), ('USA', 'America'), ('ASIA', 'Asia')]

    user = models.OneToOneField(User, on_delete=models.CASCADE)
    gender = models.CharField(default='M', choices=GENDERS, max_length=1)
    location = models.CharField(max_length=30, choices=LOCATION ,blank=True)
    birth_date = models.DateField(null=True, blank=True)

    def get_fields_values(self):
        return [(field.attname, field.value_to_string(self)) for field in Profile._meta.fields]

    def clean(self):
        from django.core.exceptions import ValidationError
        if self.birth_date > timezone.now():
            raise ValidationError({'birth_date': ('Enter a date < today.')})
        super().clean()

    def __str__(self) -> str:
        return self.user.username
