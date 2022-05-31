from django.contrib import admin
from .models import Product
from .forms import ProductForm

# Register your models here.

class ProductAdmin(admin.ModelAdmin):
    form = ProductForm

admin.site.register(Product, ProductAdmin)