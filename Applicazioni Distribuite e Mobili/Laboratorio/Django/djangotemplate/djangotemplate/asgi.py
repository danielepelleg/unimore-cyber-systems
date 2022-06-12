"""
ASGI config for djangotemplate project.

It exposes the ASGI callable as a module-level variable named ``application``.

For more information on this file, see
https://docs.djangoproject.com/en/3.2/howto/deployment/asgi/
"""

import os

from django.core.asgi import get_asgi_application

import dotenv

dotenv.load_dotenv()

PROJECT_NAME = os.environ.get('PROJECT_NAME')

os.environ.setdefault('DJANGO_SETTINGS_MODULE', f'{PROJECT_NAME}.settings')

application = get_asgi_application()
