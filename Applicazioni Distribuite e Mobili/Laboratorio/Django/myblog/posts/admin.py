from django.contrib import admin
from .forms import ArticleFullForm
from .models import Author, Article

# Register your models here.

class ArticleInline(admin.StackedInline):
    model = Article
    extra = 2

class ArticleAdmin(admin.ModelAdmin):
    form = ArticleFullForm

class AuthorArticle(admin.ModelAdmin):
    model = Author
    inlines = [ArticleInline]

admin.site.register(Author, AuthorArticle)
admin.site.register(Article, ArticleAdmin)
