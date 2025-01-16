

# test using aws cli

aws sqs send-message \
    --queue-url https://sqs.ap-northeast-2.amazonaws.com/<user_id>/<queue name> \
    --message-body "this is a test message from cli"
