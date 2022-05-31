from django.urls import include, path
from . import views
from django.conf import settings


app_name = 'products'

urlpatterns = [
    path('', views.ProductListView.as_view(), name='product_list'),
    path('<int:product_id>/buy', views.buy_product, name='purchase'),
    path('<int:pk>/', views.ProductDetailView.as_view(), name='product_detail')
]