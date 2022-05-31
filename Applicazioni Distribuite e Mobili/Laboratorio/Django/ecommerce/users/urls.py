from django.urls import path, include
from . import views

app_name = 'users'

urlpatterns = [
    path('index/', views.index, name='index'),
    path('cart/<int:user_id>', views.cart_detail, name='cart_detail'), # show cart
    path('cart/<int:user_id>/checkout', views.checkout, name='checkout'), # order
    path('cart/<int:user_id>/orders', views.OrderView.as_view(), name='orders'), # show orders
    path('cart/<int:user_id>/orders/<int:order_id>', views.order_detail, name='order_detail'), # order detail
    path('accounts/', include('django.contrib.auth.urls')),
    path('accounts/registration', views.register, name='registration')
]
