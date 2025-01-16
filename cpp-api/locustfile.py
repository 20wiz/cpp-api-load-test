from locust import HttpUser, TaskSet, task, between

url = "/count"


class UserBehavior(TaskSet):
    @task
    def my_task(self):
        self.client.get(url)  # Replace with your API endpoint

class WebsiteUser(HttpUser):
    tasks = [UserBehavior]
    wait_time = between(1, 5)