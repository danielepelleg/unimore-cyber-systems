from django.db import models

# Create your models here.

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