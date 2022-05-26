from re import A
from django.http import HttpResponse
from django.shortcuts import render
from .models import Article

# Create your views here.

def index(request):
    return HttpResponse("Articles Index")

def year_archive(request, year):
    articles_list = Article.objects.filter(pub_date__year=year)
    context = {'year': year, 'articles ': articles_list}
    return render(request, 'news/year_archive.html', context)

def month_archive(request, year, month):
    return HttpResponse(f"Detail {month} / {year} Index")

