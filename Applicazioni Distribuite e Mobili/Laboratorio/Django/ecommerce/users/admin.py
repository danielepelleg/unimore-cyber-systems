from django.contrib import admin
from .models import Customer, Cart, CartEntry, Order, OrderEntry

# Register your models here.

admin.site.register(Cart)
admin.site.register(CartEntry)
admin.site.register(Customer)
admin.site.register(Order)
admin.site.register(OrderEntry)