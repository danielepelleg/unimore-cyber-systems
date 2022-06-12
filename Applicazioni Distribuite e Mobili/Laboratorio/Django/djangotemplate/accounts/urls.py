from django.urls import path, include
from . import views

app_name = 'accounts'

urlpatterns = [
    path('', include('django.contrib.auth.urls')),
    path('registration/', views.register, name='registration'),
    # API
    path('api/users' , views.users_list_api, name='users_list_api'),
    path('api/profiles', views.profiles_list_api, name='profiles_list_api'),
]
