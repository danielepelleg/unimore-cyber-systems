# GCLOUD Cheatsheeet

## Preparation
- Activate Cloud Build API
- Activate Firebase Native
- Activate Endpoints
- Activate Cloud Functions
- Activate PubSub
- Create the credentials and store them in a *credentials.json* file

For a better navigation the GCloud Console *pin* the features needed on the sidebar.

## Generics

### Create a Project
```bash
$> gcloud projects create [PROJECT_ID] --set-as-default
```

### Delete a Project
```bash
$> gcloud projects delete [PROJECT_ID]
```

### Delete a Project with Endpoints and APIs Deployed on Flexible Environment
```bash
$> gcloud endpoints services delete [PROJECT_ID].appspot.com --project=[PROJECT_ID]
```

### Undelete a Project
```bash
$> gcloud projects undelete [PROJECT_ID]
```

### Project Description
```bash
$> gcloud projects describe [PROJECT_ID]
```

### Get Projects List
```bash
$> gcloud projects list
```

### Get the Current Project

```bash
$>gcloud config get-value core/project
# or
$> gcloud config get-value project
```

### Set the Working Project
```bash
$> gcloud config set project [PROJECT_ID]
```

### Set the Region of the Project
```bash
$>gcloud config set run/region [REGION_ID]
```
`region_id` could for example *us-central1*

## Firebase - Set Up Authentication

### Create Service Account
```bash
gcloud iam service-accounts create [NAME]
```

### Binding Service Account to Project
```bash
gcloud projects add-iam-policy-binding [PROJECT_ID] --member "serviceAccount:[NAME]@[PROJECT_ID].iam.gserviceaccount.com" --role "roles/owner"
```
`--member` specifica il tipo di account

`--role` specifica il ruolo

### Write Service Account Keys on File
Create an empty *.json* file in which store the credentials of the database `[FILE_NAME].json`
```bash
$> gcloud iam service-accounts keys create [FILE_NAME].json --iam-account [NAME]@[PROJECT_ID].iam.gserviceaccount.com
```

### Export Credentials
```bash
$> export GOOGLE_APPLICATION_CREDENTIALS="$(pwd)/[FILE_NAME].json"
```

## Deployment

### Install App-Engine Python Component 
```bash
$> gcloud components install app-engine-python
```

### Deploy
```bash
$> gcloud app deploy
```
Deploy the file `app.yaml` by default.

#### **`app.yaml`** - FRONTEND
```yaml
runtime: python37
service: default

handlers:
  - url: /.*
    secure: always
    script: auto
  
env_variables:
  PROJECT_ID:  '[PROJECT_ID]'
```

#### **`app.yaml`** - BACKEND
```yaml
runtime: python37
service: api

entrypoint: gunicorn api:app

handlers:
  - url: /.*
    secure: always
    script: auto
```
The first one to be deployed must be the configuration file with `service: default` specified. Once it has been deployed, it's possible to deploy other services such as the API.

### Deploy two Different Services
```bash
$> gcloud app deploy [FILE_1].yaml [FILE_2].yaml
```


### Dispatch
The file `dispatch.yaml` defines the routing rules of our web app. it is used to set *api path* in our web application.
```bash
$>gcloud app deploy dispatch.yaml
```

#### **`dispatch.yaml`**
```yaml
dispatch:
  - url: "[PROJECT_ID ].appspot.com/"
    service: default
  - url: "*/api/*"
    service: api
```

## Deploying Endpoints Configuration

### Validate the OpenAPI Document
```bash
$> gcloud endpoints services deploy [OPENAPI_FILE].yaml --validate-only
```

### Deploy the OpenAPI Document
```bash
$> gcloud endpoints services deploy [OPENAPI_FILE].yaml
```

## Flexible Enviroment
### Deploy Endpoints + APIs on Flexible Enviroment
```bash
$>gcloud app deploy flexenv.yaml
```

#### **`flexenv.yaml`**
```yaml
runtime: python
env: flex

entrypoint: gunicorn api:app

service: api

runtime_config:
  python_version: 3

endpoints_api_service:
  name: [PROJECT_ID].appspot.com
  rollout_strategy: managed
```
## Pub Sub

### Create a Topic
```bash
$> gcloud pubsub topics create [TOPIC_NAME]
```
More topics can be created together by just specifying their names after the first one.

### List all the Topics
```bash
$> gcloud pubsub topics list
```

### Delete a Topic
```bash
$> gcloud pubsub topics delete [TOPIC_NAME]
```

### Get Topic's Description
```bash
$> gcloud pubsub topics describe [TOPIC_NAME]
```

### Pub su un Topic
```bash
$> gcloud pubsub topics publish [TOPIC_NAME] --attribute=from="cli",to="someone" --message="ciao a tutti"
```
It returns the ID of the message sent. After the `attribute=` a list of couples key-value is specified. Watch out there is no space between `,` and `to`.

### Create a Subscription
```bash
$> gcloud pubsub subscriptions create [SUBSCRIPTION_NAME] --topic [TOPIC_NAME]
```
If something is sent before the subscription, it is not sent to the subscriber once he subscribes.

### Get the Subscription List
```bash
$> gcloud pubsub subscriptions list
```

### Retrieve Subscription Message
```bash
$> gcloud pubsub subscriptions pull [SUBSCRIPTION_NAME]
```

### Create a Subscription with Endpoint
```bash
$> gcloud pubsub subscriptions create [SUBSCRIPTION_NAME] --topic [TOPIC_NAME] --push-endpoint "https://[PROJECT_NAME].appspot.com/[PATH]?token=[TOKEN_VALUE]" --ack-deadline [SECONDS]
```
After the time specified in the ack deadline the message is considered as lost.

### See Logs
```bash
$> gcloud app logs tail -s default
```

### Send a Request
```bash
$> gcloud pubsub topics publish [TOPIC_NAME] --attribute=prova='si' --message="test di pubblicazione"
```
Returns the message ID of the request sent.

## Functions

### Deploying a Function Triggered by HTTP
```bash
$> gcloud functions deploy [FUNCTION_NAME] --runtime [RUNTIME] --trigger-http --allow-unauthenticated
```
The function specified by `function_name` is searched by gcloud inside a `main.py` file by default, because we specify as runtime python, it will be looking for a `.py` file. when we specify HTTP as a trigger is to obtain a request object. The `runtime` can be for example *python37*.

### Call a Function triggered by HTTP
```bash
$> gcloud functions call [FUNCTION_NAME] --data '{"name": "[NAME]"}'
```

### Deploy a Function Triggered by a Message on a Topic 
```bash
$> gcloud functions deploy [FUNCTION_NAME] --trigger-topic [TOPIC_NAME] --runtime [RUNTIME]
```
Function triggered by receiving a message on a topic.


### Function Triggered by a Firestore Event
```bash
$> gcloud functions deploy [FUNCTION_NAME] --env-vars-file .env.yaml --runtime [RUNTIME] --trigger-event "providers/cloud.firestore/eventTypes/document.write" --trigger-resource "projects/[PROJECT_ID]/databases/(default)/documents/{document}/{attribute}"
```
The trigger event specify the action that triggers the event. In this case, by specifying `document.write` the event is triggered after a write operation. On Linux system better to specify `--runtime=[RUNTIME]`
