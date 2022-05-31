from django.db import models
from django.utils import timezone
from django.core.validators import MinValueValidator

# Create your models here.

class Product(models.Model):
    CATEGORIES = [('F', 'Food'), 
        ('C', 'Clothes'),
        ('T', 'Technology'),
        ('O', 'Other')]

    title = models.CharField(max_length=200)
    category = models.CharField(max_length=1, choices=CATEGORIES)
    price = models.DecimalField(default=0.00, max_digits=5, decimal_places=2)
    pubblication_date = models.DateField(default=timezone.now)
    quantity = models.IntegerField(default=0, validators=[MinValueValidator(1)])
    image = models.ImageField(upload_to='images')

    def clean(self):
        from django.core.exceptions import ValidationError
        if  self.quantity <= 0:
            raise ValidationError({'quantity': ('Enter a value >= 0.')})  
        super(Product, self).clean()

    def __str__(self) -> str:
        return self.title

