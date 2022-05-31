from django.forms import ModelForm
from .models import Customer
from products.forms import DateInput

class CustomerForm(ModelForm):
    class Meta:
        model = Customer
        fields = ['gender', 'location', 'birth_date']
        widgets = {
            'birth_date': DateInput()
        }