#include "zMessageTest.h"

#include <zutils/zInet.h>

using namespace Test;
using namespace zUtils;

int zMessageTest_MessageHandler(void* arg_)
{

    ZLOG_DEBUG("#############################################################");
    ZLOG_DEBUG("# zMessageTest_MessageHandler()");
    ZLOG_DEBUG("#############################################################");

    // Setup network socket
    zSocket::InetAddress MyAddr("lo");
    TEST_TRUE(MyAddr.SetPort("8888"));

    // Create new socket and validate
    zSocket::InetSocket *MySock = new zSocket::InetSocket(MyAddr);
    TEST_ISNOT_NULL(MySock);

    // Create new message socket and validate
    zMessage::MessageSocket *MyMsgSock = new zMessage::MessageSocket(MySock);
    TEST_ISNOT_NULL(MyMsgSock);

    // Create new message handler and validate
    TestHandler *MyHandler = new TestHandler(MyMsgSock);
    TEST_ISNOT_NULL(MyHandler);

    // Startup the message handler
    TEST_TRUE(MySock->Open());
    TEST_TRUE(MySock->Bind());

    // Broadcast hello message to self
    zMessage::Message *helloMsg = zMessage::Factory::Create(zMessage::Message::TYPE_HELLO);
    TEST_ISNOT_NULL(helloMsg);
    TEST_TRUE(helloMsg->SetId("helloMsg"));
    TEST_TRUE(helloMsg->SetDst(MyAddr.GetAddress()));
    TEST_TRUE(helloMsg->SetSrc(MyAddr.GetAddress()));
    ZLOG_DEBUG(helloMsg->GetJson());
    TEST_TRUE(MyMsgSock->Send(*helloMsg));
    delete (helloMsg);

    // Wait for message to be sent
    TEST_TRUE(MyHandler->TxSem.TimedWait(10000));

    // No more messages should have been sent
//    TEST_FALSE(MyHandler->TxSem.TryWait());

    // Verify no errors
    TEST_FALSE(MyHandler->ErrSem.TryWait());

    // Wait for message to be received
    TEST_TRUE(MyHandler->RxSem[zMessage::Message::TYPE_HELLO].TimedWait(10000));

    // No more messages should be waiting
    TEST_FALSE(MyHandler->RxSem[zMessage::Message::TYPE_HELLO].TryWait());

    // Verify no errors
    TEST_FALSE(MyHandler->ErrSem.TryWait());

    // Acknowledge hello message
    zMessage::Message *ackMsg = zMessage::Factory::Create(zMessage::Message::TYPE_ACK);
    TEST_ISNOT_NULL(ackMsg);
    TEST_TRUE(ackMsg->SetId("ackMsg"));
    TEST_TRUE(ackMsg->SetDst(MyAddr.GetAddress()));
    TEST_TRUE(ackMsg->SetSrc(MyAddr.GetAddress()));
    ZLOG_DEBUG(ackMsg->GetJson());
    TEST_TRUE(MyMsgSock->Send(*ackMsg));
    delete (ackMsg);

    // Wait for message to be sent
    TEST_TRUE(MyHandler->TxSem.TimedWait(10000));

    // No more messages should have been sent
//    TEST_FALSE(MyHandler->TxSem.TryWait());

    // Verify no errors
    TEST_FALSE(MyHandler->ErrSem.TryWait());

    // Wait for message to be received
    TEST_TRUE(MyHandler->RxSem[zMessage::Message::TYPE_ACK].TimedWait(10000));

    // No more messages should be waiting
    TEST_FALSE(MyHandler->RxSem[zMessage::Message::TYPE_ACK].TryWait());

    // Verify no errors
    TEST_FALSE(MyHandler->ErrSem.TryWait());

    // Send bye message to self
    zMessage::Message *byeMsg = zMessage::Factory::Create(zMessage::Message::TYPE_BYE);
    TEST_ISNOT_NULL(byeMsg);
    TEST_TRUE(byeMsg->SetId("byeMsg"));
    TEST_TRUE(byeMsg->SetDst(MyAddr.GetAddress()));
    TEST_TRUE(byeMsg->SetSrc(MyAddr.GetAddress()));
    ZLOG_DEBUG(byeMsg->GetJson());
    TEST_TRUE(MyMsgSock->Send(*byeMsg));
    delete (byeMsg);

    // Wait for message to be sent
    TEST_TRUE(MyHandler->TxSem.TimedWait(10000));

    // No more messages should have been sent
//    TEST_FALSE(MyHandler->TxSem.TryWait());

    // Verify no errors
    TEST_FALSE(MyHandler->ErrSem.TryWait());

    // Wait for message to be received
    TEST_TRUE(MyHandler->RxSem[zMessage::Message::TYPE_BYE].TimedWait(10000));

    // No more messages should be waiting
    TEST_FALSE(MyHandler->RxSem[zMessage::Message::TYPE_BYE].TryWait());

    // Verify no errors
    TEST_FALSE(MyHandler->ErrSem.TryWait());

    // Clean up
    delete (MyHandler);
    delete(MyMsgSock);
    MySock->Close();
    delete (MySock);

    // Return success
    return (0);

}

