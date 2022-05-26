"""mysite URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/3.2/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from argparse import Namespace
from django import views
from django.contrib import admin
from django.urls import path, include
from django.contrib.auth import views as auth_views, urls
from django.conf import settings
from django.contrib.auth.views import logout_then_login
from . import views

urlpatterns = [
    path('', views.main_page, name='index'),
    path('', include('django.contrib.auth.urls')),
    path('logout/', views.logout_view, name='logout'),
    path('books/', include('books.urls', namespace='books')),
    path('news/', include('news.urls', namespace='articles')),
    path('polls/', include('polls.urls', namespace='polls')),
    path('portal/', include('portal.urls', namespace='portal')),
    path('admin/', admin.site.urls),
]
