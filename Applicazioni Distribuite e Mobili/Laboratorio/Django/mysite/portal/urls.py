from django.urls import path, re_path
from . import views

app_name = 'portal'

urlpatterns = [
    path('', views.portal_welcome, name='welcome'),
    path('contact/', views.contact, name='contact'),
    path('register/', views.register, name='register')
]