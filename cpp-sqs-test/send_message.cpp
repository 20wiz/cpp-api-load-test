#include <aws/core/Aws.h>
#include <aws/sqs/SQSClient.h>
#include <aws/sqs/model/SendMessageRequest.h>
#include <iostream> 

int main() {
    Aws::SDKOptions options;
    Aws::InitAPI(options);
    {
        const std::string queue_url = "https://sqs.ap-northeast-2.amazonaws.com/935659127831/myq1";
        // const std::string queue_url = "https://sqs.us-east-1.amazonaws.com/YOUR_ACCOUNT_ID/YOUR_QUEUE_NAME";

        Aws::SQS::SQSClient sqs;
        Aws::SQS::Model::SendMessageRequest request;
        request.SetQueueUrl(queue_url);
        request.SetMessageBody("Hello from AWS SDK for C++!");

        auto outcome = sqs.SendMessage(request);

        if (outcome.IsSuccess()) {
            std::cout << "Message sent successfully. Message ID: "
                      << outcome.GetResult().GetMessageId() << std::endl;
        } else {
            std::cerr << "Error sending message: "
                      << outcome.GetError().GetMessage() << std::endl;
        }
    }
    Aws::ShutdownAPI(options);
    return 0;
}
