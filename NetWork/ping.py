#coding:utf-8
import os
import time

import select

__author__ = 'qiye'
__date__ = '2018/5/6 15:53'

import socket
import struct,sys


def checksum(packet):
    """
    校验
    """
    #packet为icmp头部和data的字节流，其中icmp校验和字段初始化为0
    sum =0
    #countTo:记录packet是有多少个16位，因为对每两个字节进行校验
    countTo = (len(packet)//2)*2
    count =0
    while count <countTo:
        #将每两个字节中的第二个字节作为高位，第一个字节作为低位组成16位的值
        sum += ((packet[count+1] << 8) | packet[count])
        count += 2
    #packet并不一定都是偶数字节，可能是奇数，把最后一个字节加到sum中
    if countTo<len(packet):
        sum += packet[len(packet) - 1]
        sum = sum & 0xffffffff

    #sum中超过16位的高位加到低位
    sum = (sum >> 16)  +  (sum & 0xffff)
    sum = sum + (sum >> 16)
    #对sum取反
    answer = ~sum

    #到这应该就结束了，但是字节序有问题，下面是将主机字节序转为网络字节序即高位转低位，低位转高位
    answer = answer & 0xffff
    answer = answer >> 8 | (answer << 8 & 0xff00)
    return answer

def send_one_ping(rawsocket, dst_addr, icmp_id, icmp_sq):

    dst_addr = socket.gethostbyname(dst_addr)
    packet = struct.pack('!BBHHH32s', 8, 0, 0, icmp_id, icmp_sq, b'abcdefghijklmnopqrstuvwabcdefghi')
    chksum=checksum(packet)
    packet = struct.pack('!BBHHH32s', 8, 0, chksum,icmp_id, icmp_sq, b'abcdefghijklmnopqrstuvwabcdefghi')
    send_time = time.time()
    rawsocket.sendto(packet, (dst_addr, 80))
    return send_time,dst_addr

def recv_one_ping(rawsocket,icmp_id, icmp_sq ,time_sent,timeout):
    while True:
        started_select = time.time()
        what_ready = select.select([rawsocket], [], [], timeout)
        how_long_in_select = (time.time() - started_select)
        if what_ready[0] == []:  # Timeout
            return -1
        time_received = time.time()
        received_packet, addr = rawsocket.recvfrom(1024)
        icmpHeader = received_packet[20:28]
        type, code, checksum, packet_id, sequence = struct.unpack(
            "!BBHHH", icmpHeader
        )
        if type == 0 and packet_id == icmp_id and sequence == icmp_sq:
            return time_received - time_sent
        timeout = timeout - how_long_in_select
        if timeout <= 0:
            return -1

def one_ping(dst_addr,icmp_sq,timeout = 2):
    try:
        rawsocket = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.getprotobyname("icmp"))
    except socket.error as e:
        if e.errno == 1:
            msg = e.message + (" please run as root ")
            raise socket.error(msg)
        raise

    icmp_id = os.getpid() & 0xFFFF

    send_time,addr = send_one_ping(rawsocket, dst_addr, icmp_id, icmp_sq)
    time = recv_one_ping(rawsocket, icmp_id, icmp_sq, send_time, timeout)
    return time,addr



def ping(dst_addr,timeout = 2, count = 4):
    for i in range(0,count):
        time, addr = one_ping(dst_addr,i+1,timeout)
        if time > 0:
            print("来自 {0} 的回复: 字节=32 时间={1}ms".format(addr,int(time*1000)))
        else:
            print("来自 {0} 的回复:超时".format(addr))



if __name__=="__main__":
    if len(sys.argv) < 2:
        sys.exit('Usage: ping.py <hostname>')
    ping(sys.argv[1])









