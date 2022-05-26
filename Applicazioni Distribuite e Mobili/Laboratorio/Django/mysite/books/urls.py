from django.urls import path
from . import views

app_name = 'books'

urlpatterns = [
    path('insertAuth', views.insertAuth, name='insertAuth'),
    path('modifyAuth/<int:author_id>', views.modifyAuth, name='modifyAuth')
]
