from django.forms import ModelForm, DateInput
from .models import Product 

class DateInput(DateInput):
    input_type = 'date'

class ProductForm(ModelForm):
    class Meta:
        model = Product
        fields = ['id', 'title', 'category', 'price', 'pubblication_date', 'quantity', 'image']
        labels = {
            'pubblication_date': 'Pubblication Date'
        }
        widgets = {
            'pubblication_date': DateInput()
        }