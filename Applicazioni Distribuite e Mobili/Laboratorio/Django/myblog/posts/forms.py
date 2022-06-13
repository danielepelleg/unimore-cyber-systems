from django.forms import ModelForm, DateInput
from .models import Author, Article

class DateInput(DateInput):
    input_type = 'date'

class AuthorFullForm(ModelForm):
    class Meta:
        model = Author
        fields = ['id', 'user', 'full_name', 'title', 'nationality']
        labels = {
            'full_name': 'Full Name'
        }

class AuthorForm(ModelForm):
    class Meta:
        model = Author
        fields = ['id', 'full_name', 'title', 'nationality']
        labels = {
            'full_name': 'Full Name'
        }
 
 
class ArticleFullForm(ModelForm):
    class Meta:
        model = Article
        fields = ['id', 'title', 'content', 'author']
        # labels = {
        #     'pubblication_date': 'Pubblication Date'
        # }
        # widgets = {
        #     #'pubblication_date': DateInput(),
        #     #'author': CheckboxSelectMultiple()
        # }

class ArticleForm(ModelForm):
    class Meta:
        model = Article
        fields = ['title', 'content']