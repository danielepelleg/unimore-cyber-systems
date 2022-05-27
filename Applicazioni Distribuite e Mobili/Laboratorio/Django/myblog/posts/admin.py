from django.contrib import admin
from .forms import ArticleForm
from .models import Author, Article

# Register your models here.

class ArticleInline(admin.StackedInline):
    model = Article
    extra = 2

class ArticleAdmin(admin.ModelAdmin):
    form = ArticleForm

admin.site.register(Author)
admin.site.register(Article, ArticleAdmin)
