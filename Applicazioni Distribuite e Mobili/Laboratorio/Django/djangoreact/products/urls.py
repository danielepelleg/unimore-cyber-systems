from django.urls import path
from . import views

app_name = 'products'

urlpatterns = [
    path('', views.products_list_api, name='list_api'),
    path('index/', views.index, name='index'),
    path('jquery/', views.products_list_jquery, name='list_jquery'),
    path('react/', views.products_list_react, name='list_react'),
    path('django/', views.ProductListView.as_view(), name='list_django')
]
