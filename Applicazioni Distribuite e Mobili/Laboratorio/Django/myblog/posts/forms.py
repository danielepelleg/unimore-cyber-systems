from django.forms import ModelForm, DateInput, CheckboxSelectMultiple
from .models import Author, Article

class DateInput(DateInput):
    input_type = 'date'

class AuthorForm(ModelForm):
    class Meta:
        model = Author
        fields = ['id', 'full_name', 'title', 'nationality']
        labels = {
            'full_name': 'Full Name'
        }
 
class ArticleForm(ModelForm):
    class Meta:
        model = Article
        fields = ['id', 'title', 'content', 'pubblication_date', 'author']
        labels = {
            'pubblication_date': 'Pubblication Date'
        }
        widgets = {
            'pubblication_date': DateInput(),
            'author': CheckboxSelectMultiple()
        }