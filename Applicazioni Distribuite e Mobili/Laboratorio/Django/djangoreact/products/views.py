from django.http import JsonResponse
from django.shortcuts import render
from django.views import generic
from django.views.decorators.csrf import csrf_exempt
from .models import Product
from .serializers import ProductSerializer

# Create your views here.

def index(request):
    return render(request, 'products/index.html')

class ProductListView(generic.ListView):
    template_name = 'products/list.html'
    context_object_name = "elements_list"

    def get_queryset(self) -> list:
        return Product.objects.all()

def products_list_jquery(request):
    url = f"/products/"
    return render(request, 'products/jquery.html', {'url': url, 'script_name': 'list.js'})

def products_list_react(request):
    return render(request, 'index.html')

"""
    API
"""
@csrf_exempt
def products_list_api(request):
    if request.method == 'GET':
        products = Product.objects.all()
        serializer = ProductSerializer(products, many=True)
        return JsonResponse(serializer.data, safe=False)