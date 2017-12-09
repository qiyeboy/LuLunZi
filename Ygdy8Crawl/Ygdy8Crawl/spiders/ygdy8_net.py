# -*- coding: utf-8 -*-
import os
import re

import scrapy
from scrapy.linkextractors import LinkExtractor
from scrapy.spiders import CrawlSpider, Rule

from Ygdy8Crawl.items import Ygdy8CrawlItem


class Ygdy8NetSpider(CrawlSpider):
    name = 'ygdy8.net'
    allowed_domains = ['ygdy8.net','dytt8.net']
    start_urls = ['http://www.ygdy8.net/html/gndy/rihan/index.html',
                  'http://www.ygdy8.net/html/gndy/oumei/index.html',
                  'http://www.ygdy8.net/html/gndy/china/index.html',
                  'http://www.ygdy8.net/html/gndy/dyzz/index.html',
                  'http://www.ygdy8.net/html/gndy/jddy/index.html',
                  'http://www.dytt8.net/html/tv/hytv/index.html',
                  'http://www.dytt8.net/html/tv/rihantv/index.html',
                  'http://www.dytt8.net/html/tv/oumeitv/index.html',
                  'http://www.dytt8.net/html/tv/gangtai/index.html',
                  'http://www.dytt8.net/html/tv/hepai/index.html',
                  'http://www.dytt8.net/html/zongyi2013/index.html',
                  'http://www.dytt8.net/html/2009zongyi/index.html',
                  'http://www.dytt8.net/html/dongman/index.html']

    def __init__(self, *args, **kwargs):
        super(Ygdy8NetSpider, self).__init__(*args, **kwargs)
        self.urlset = set()#用来存储url+time
        # 在启动之前首先加载urlset池，用来去重
        if os.path.exists('urlset.txt'):
            with open("urlset.txt",'r') as fp:
                for line in fp.readlines():
                    if len(line)>0:
                        self.urlset.add(line)
        else:
            open("urlset.txt",'w')
    rules = (
        #解析movie
        Rule(LinkExtractor(allow=r'/html/gndy/rihan/list_\d+_\d+.html'), callback='parse_list', follow=True),
        Rule(LinkExtractor(allow=r'/html/gndy/oumei/list_\d+_\d+.html'), callback='parse_list', follow=True),
        Rule(LinkExtractor(allow=r'/html/gndy/china/list_\d+_\d+.html'), callback='parse_list', follow=True),
        Rule(LinkExtractor(allow=r'/html/gndy/dyzz/list_\d+_\d+.html'), callback='parse_list', follow=True),
        Rule(LinkExtractor(allow=r'/html/gndy/jddy/list_\d+_\d+.html'), callback='parse_list', follow=True),
        #解析电视剧
        Rule(LinkExtractor(allow=r'/html/tv/hytv/list_\d+_\d+.html'), callback='parse_list', follow=True),
        Rule(LinkExtractor(allow=r'/html/tv/rihantv/list_\d+_\d+.html'), callback='parse_list', follow=True),
        Rule(LinkExtractor(allow=r'/html/tv/oumeitv/list_\d+_\d+.html'), callback='parse_list', follow=True),
        Rule(LinkExtractor(allow=r'/html/tv/gangtai/list_\d+_\d+.html'), callback='parse_list', follow=True),
        Rule(LinkExtractor(allow=r'/html/tv/hepai/list_\d+_\d+.html'), callback='parse_list', follow=True),
        #解析综艺
        Rule(LinkExtractor(allow=r'/html/zongyi2013/list_\d+_\d+.html'), callback='parse_list', follow=True),
        Rule(LinkExtractor(allow=r'/html/2009zongyi/list_\d+_\d+.html'), callback='parse_list', follow=True),
        #解析动漫
        Rule(LinkExtractor(allow=r'/html/dongman/list_\d+_\d+.html'), callback='parse_list', follow=True),

    )

    def parse_list(self, response):
        #用来解析电影链接
        url_date =''
        movies = response.css(".tbspan")

        for movie in movies:

            movieUrl = movie.css ('a[class="ulink"]::attr(href)').extract()#抽取出电影链接
            if len(movieUrl)>1:
                movieUrl = movieUrl[1]
            else:
                movieUrl = movieUrl[0]
            movieUrl = response.urljoin(url=movieUrl)

            title = movie.css('a[class="ulink"]::text').extract() # 抽取电影标题
            if len(title)>1:
                title = title[1]
            else:
                title = title[0]
            d_title = title
            d_remarks=''
            try:
                d_name = title[title.index(u"《")+1:title.index(u"》")]#电影名称
                d_remarks = title[title.index(u"》")+1:] #电影备注
            except Exception as e:
                try:
                    d_name = title[title.index(u"《") + 1:title.index(u"】")]  # 电影名称
                    d_remarks = title[title.index(u"】") + 1:]  # 电影备注
                except Exception as e1:
                    pattern = re.compile(u'\[([\s\S]*?)\]([\s\S]*)')
                    result = re.search(pattern, d_title)
                    if result is not None:
                        d_name = result.groups()
                        if d_name > 1:
                            d_name = d_name[1].strip()
                        else:
                            d_name = title
                            d_remarks = d_name

                    else:
                        d_name = title
                        d_remarks = d_name


            date = movie.css('tr>td>font::text').extract_first()
            #从日期和点击之间抽取出更新时间
            date = date[date.index(u"日期")+3:date.index(u"点击")]#获取更新时间
            flag = (movieUrl+date).strip()+'\n'
            if flag in self.urlset:
                continue
            else:
                print flag
                self.urlset.add(flag)
                url_date+=flag
                request = scrapy.Request(url=movieUrl,callback=self.parse_body)
                request.meta['movieUrl'] = movieUrl
                request.meta['d_title'] = d_title
                request.meta['d_name'] = d_name
                request.meta['d_remarks'] = d_remarks
                yield request
        #将flags写入文件
        if len(url_date)>0:
            with open(r'urlset.txt','a') as fw:
                fw.write(url_date)
                url_date=''


    def parse_body(self,response):
        if 'gndy' in response.url:
            return self.parse_movie(response)

        elif 'tv' in response.url:
            return self.parse_tv(response)
        elif 'zongyi' in response.url:
            return self.parse_zongyi(response)
        elif 'dongman'in response.url:
            return self.parse_donman(response)
        else:
            return self.parse_movie(response)




    def parse_movie(self,response):
        #用来解析电影内容
        ygdy8_item = Ygdy8CrawlItem()
        movieUrl = response.meta['movieUrl']
        d_name = response.meta['d_name']
        d_remarks = response.meta['d_remarks']
        d_title = response.meta['d_title']
        #下面解析电影的其他内容
        ##Zoom>span>img提取图片

        content = response.xpath(".//*[@id='Zoom']")
        content = content[0].xpath("string(.)").extract()[0].strip().replace(r'\n','')
        try:
            pattern = re.compile(u"◎译[\s\S]*?名([\s\S]*?)◎")
            result = re.search(pattern, content)
            d_subname = result.groups()[0].strip()#副标题
        except Exception as e:
            d_subname=''

        try:
            pattern = re.compile(u"◎类[\s\S]*?别([\s\S]*?)◎")
            result = re.search(pattern, content)
            d_type = result.groups()[0].strip()#
        except Exception as e:
            d_type=''

        for item in [u"◎产[\s\S]*?地([\s\S]*?)◎",u"◎国[\s\S]*?家([\s\S]*?)◎",u"◎地[\s\S]*?区([\s\S]*?)◎",'']:
            if len(item)<1:
                d_area = ''
                break
            pattern = re.compile(item)
            result = re.search(pattern, content)
            if result is None:
                continue
            d_area = result.groups()
            if d_area>0:
                d_area = d_area[0].strip()
                break



        try:
            pattern = re.compile(u"◎语[\s\S]*?言([\s\S]*?)◎")
            result = re.search(pattern, content)
            d_lang = result.groups()[0].strip()#
        except Exception as e:
            d_lang=''

        try:
            pattern = re.compile(u"◎年[\s\S]*?代([\s\S]*?)◎")
            result = re.search(pattern, content)
            d_year = result.groups()[0].strip()#年代
        except Exception as e:
            d_year=0

        try:
            pattern = re.compile(u"◎主[\s\S]*?演([\s\S]*?)◎")
            result = re.search(pattern, content)
            d_starring = result.groups()[0].strip()#
        except Exception as e:
            try:
                pattern = re.compile(u"◎演[\s\S]*?员([\s\S]*?)◎")
                result = re.search(pattern, content)
                d_starring = result.groups()[0].strip()#
            except Exception as e1:
                d_starring = ''


        try:
            pattern = re.compile(u"◎导[\s\S]*?演([\s\S]*?)◎")
            result = re.search(pattern, content)
            d_directed = result.groups()[0].strip()#
        except Exception as e:
            d_directed=''

        try:
            pattern = re.compile(u"◎片[\s\S]*?长([\s\S]*?)◎")
            result = re.search(pattern, content)
            d_duration = result.groups()[0].strip()#
        except Exception as e:
            d_duration=0

        try:
            pattern = re.compile(u"◎简[\s\S]*?介([\s\S]*?)【下载地址】")
            result = re.search(pattern, content)
            d_content = result.groups()[0].strip()#
        except Exception as e:
            d_content=''

        #解析图片链接
        pic_list = response.css("#Zoom img::attr(src)").extract()
        if len(pic_list)>1 :
            d_pic = pic_list[0]
            d_contentpic = pic_list[1]
        elif len(pic_list)==1:
            d_pic = pic_list[0]
            d_contentpic=''
        else:
            d_pic = ''
            d_contentpic=''

        d_downurl = response.css("#Zoom a[href^=ftp]::attr(href)").extract()
        d_class=''
        ygdy8_item['movieUrl']=movieUrl
        ygdy8_item['d_name'] = d_name
        ygdy8_item['d_remarks'] = d_remarks
        ygdy8_item['d_subname'] = d_subname
        ygdy8_item['d_type'] = d_type
        ygdy8_item['d_area'] = d_area
        ygdy8_item['d_lang'] = d_lang
        ygdy8_item['d_starring'] = d_starring
        ygdy8_item['d_directed'] = d_directed
        ygdy8_item['d_duration'] = d_duration
        ygdy8_item['d_content'] = d_content
        ygdy8_item['d_pic'] = d_pic
        ygdy8_item['d_contentpic'] = d_contentpic
        ygdy8_item['d_downurl'] = d_downurl
        ygdy8_item['d_year']=d_year
        ygdy8_item['d_class']=d_class
        yield ygdy8_item


    def parse_tv(self,response):
        ygdy8_item = Ygdy8CrawlItem()
        movieUrl = response.meta['movieUrl']
        d_name = response.meta['d_name']
        d_remarks = response.meta['d_remarks']
        d_title = response.meta['d_title']
        d_subname = d_type= d_area= d_lang = d_starring= d_directed = d_duration = d_content=d_year=''
        #下面解析电影的其他内容
        ##Zoom>span>img提取图片

        content = response.xpath(".//*[@id='Zoom']")
        content = content[0].xpath("string(.)").extract()[0].strip().replace(r'\n','')

        for item in [u"◎译[\s\S]*?名([\s\S]*?)◎",u'\[剧[\s\S]*?名\]:([\s\S]*?)\[',u'【译[\s\S]*?名】：([\s\S]*?)【','']:
            if len(item)<1:
                d_subname = d_name
                break
            pattern = re.compile(item)
            result = re.search(pattern, content)
            if result is None:
                continue
            d_subname = result.groups()
            if d_subname>0:
                d_subname = d_subname[0].strip()
                break


        for item in [u"◎类[\s\S]*?别([\s\S]*?)◎",u'\[类[\s\S]*?型\]:([\s\S]*?)\[',u'【类[\s\S]*?别】：([\s\S]*?)【','']:
            if len(item)<1:
                d_type = ''
                break
            pattern = re.compile(item)
            result = re.search(pattern, content)
            if result is None:
                continue
            d_type = result.groups()
            if d_type>0:
                d_type = d_type[0].strip()
                break


        for item in [u"◎产[\s\S]*?地([\s\S]*?)◎",u"◎国[\s\S]*?家([\s\S]*?)◎",u"◎地[\s\S]*?区([\s\S]*?)◎",u'【国[\s\S]*?家】：([\s\S]*?)【','']:
            if len(item)<1:
                d_area = ''
                break
            pattern = re.compile(item)
            result = re.search(pattern, content)
            if result is None:
                continue
            d_area = result.groups()
            if d_area>0:
                d_area = d_area[0].strip()
                break

        for item in [u"◎语[\s\S]*?言([\s\S]*?)◎",u'【语[\s\S]*?言】：([\s\S]*?)【','']:
            if len(item)<1:
                if u'国语' in d_title:
                    d_lang = u'国语'
                elif u"韩语" in d_title:
                    d_lang = u'韩语'
                elif u'日语' in d_title:
                    d_lang = u'日语'
                elif u'中英' in d_title:
                    d_lang = u'英语'
                else:
                    d_lang = ''
                break
            pattern = re.compile(item)
            result = re.search(pattern, content)
            if result is None:
                continue
            d_lang = result.groups()
            if d_lang>0:
                d_lang = d_lang[0].strip()
                break

        if len(d_area)<1:
            if u'台湾'in d_title:
                d_area = u'台湾'
            elif u"香港" in d_title:
                d_area = u'香港'
            elif u'美剧' in d_title:
                d_area = u'美国'
            elif u"韩语" in d_title:
                d_area = u'韩国'
            elif u'日语' in d_title:
                d_area = u'日本'
            elif u"国语" in d_title:
                d_area = u'大陆'
            else:
                d_area = ''



        for item in [u'【首[\s\S]*?播】：([\s\S]*?)【',u'\[首[\s\S]*?播\]:[\s\S]*?(\d+)[\s\S]*?\[',u"◎年[\s\S]*?代([\s\S]*?)◎",u'[\s\S] * ?(\d +)[\s\S] * ?','']:
            if len(item)<1:
                d_year = 0
                break
            pattern = re.compile(item)
            result = re.search(pattern, content)
            if result is None:
                continue
            d_year = result.groups()
            if d_year>0:
                d_year = d_year[0].strip()
                break



        for item in [u"◎主[\s\S]*?演([\s\S]*?)◎",u"◎演[\s\S]*?员([\s\S]*?)◎",u'【演[\s\S]*?员】：([\s\S]*?)【',u'\[演[\s\S]*?员\]:([\s\S]*?)\[','']:
            if len(item)<1:
                d_starring = ''
                break
            pattern = re.compile(item)
            result = re.search(pattern, content)
            if result is None:
                continue
            d_starring = result.groups()
            if d_starring>0:
                d_starring = d_starring[0].strip()
                break

        for item in [u'【导[\s\S]*?演】：([\s\S]*?)【',u"◎导[\s\S]*?演([\s\S]*?)◎",u'\[导[\s\S]*?演\]:([\s\S]*?)\[','']:
            if len(item)<1:
                d_directed = ''
                break
            pattern = re.compile(item)
            result = re.search(pattern, content)
            if result is None:
                continue
            d_directed = result.groups()
            if d_directed>0:
                d_directed = d_directed[0].strip()
                break

        for item in [u'【片[\s\S]*?长】：[\s\S]*?(\d+)[\s\S]*?【',u"◎片[\s\S]*?长([\s\S]*?)◎",'']:
            if len(item)<1:
                d_duration = 0
                break
            pattern = re.compile(item)
            result = re.search(pattern, content)
            if result is None:
                continue
            d_duration = result.groups()
            if d_duration>0:
                d_duration = d_duration[0].strip()
                break

        for item in [u"【简[\s\S]*?介】：([\s\S]*?)【下载地址】",u"◎简[\s\S]*?介([\s\S]*?)【下载地址】",u'\[简[\s\S]*?介\]:([\s\S]*?)【','']:
            if len(item)<1:
                d_content = ''
                break
            pattern = re.compile(item)
            result = re.search(pattern, content)
            if result is None:
                continue
            d_content = result.groups()
            if d_content>0:
                d_content = d_content[0].strip()
                break


        #解析图片链接
        pic_list = response.css("#Zoom img::attr(src)").extract()
        if len(pic_list)>1 :
            d_pic = pic_list[0]
            d_contentpic = pic_list[1]
        elif len(pic_list)==1:
            d_pic = pic_list[0]
            d_contentpic=''
        else:
            d_pic = ''
            d_contentpic=''

        d_downurl = response.css("#Zoom a[href^=ftp]::attr(href)").extract()
        d_class=''
        ygdy8_item['movieUrl']=movieUrl
        ygdy8_item['d_name'] = d_name
        ygdy8_item['d_remarks'] = d_remarks
        ygdy8_item['d_subname'] = d_subname
        ygdy8_item['d_type'] = d_type
        ygdy8_item['d_area'] = d_area
        ygdy8_item['d_lang'] = d_lang
        ygdy8_item['d_starring'] = d_starring
        ygdy8_item['d_directed'] = d_directed
        ygdy8_item['d_duration'] = d_duration
        ygdy8_item['d_content'] = d_content
        ygdy8_item['d_pic'] = d_pic
        ygdy8_item['d_contentpic'] = d_contentpic
        ygdy8_item['d_downurl'] = d_downurl
        ygdy8_item['d_year']=d_year
        ygdy8_item['d_class']=d_class
        yield ygdy8_item

    def parse_zongyi(self,response):

        ygdy8_item = Ygdy8CrawlItem()
        movieUrl = response.meta['movieUrl']
        d_name = response.meta['d_name']
        d_remarks = response.meta['d_remarks']
        d_title = response.meta['d_title']

        #下面解析电影的其他内容
        ##Zoom>span>img提取图片

        content = response.xpath(".//*[@id='Zoom']")
        content = content[0].xpath("string(.)").extract()[0].strip().replace(r'\n','')
        d_subname = ''

        pattern = re.compile(u'\[([\s\S]*?)\][\s\S]*?(\d{4})[\s\S]*?')
        result = re.search(pattern, d_title)
        d_type=''
        d_year=0
        if result is not None:
            d_type = result.groups()
            if d_type > 1:
                d_type = d_type[0].strip()
                d_year = d_type[1].strip()
        if u"台湾综艺" in d_type or u"香港综艺" in d_type:
            d_type = u"港台综艺"


        d_area = ''
        d_lang = ''

        d_starring = ''
        d_directed = ''
        d_duration = 0
        d_content = content

        #解析图片链接
        pic_list = response.css("#Zoom img::attr(src)").extract()
        if len(pic_list)>1 :
            d_pic = pic_list[0]
            d_contentpic = pic_list[1]
        elif len(pic_list)==1:
            d_pic = pic_list[0]
            d_contentpic=''
        else:
            d_pic = ''
            d_contentpic=''

        d_downurl = response.css("#Zoom a[href^=ftp]::attr(href)").extract()
        d_class=''
        ygdy8_item['movieUrl']=movieUrl
        ygdy8_item['d_name'] = d_name
        ygdy8_item['d_remarks'] = d_remarks
        ygdy8_item['d_subname'] = d_subname
        ygdy8_item['d_type'] = d_type
        ygdy8_item['d_area'] = d_area
        ygdy8_item['d_lang'] = d_lang
        ygdy8_item['d_starring'] = d_starring
        ygdy8_item['d_directed'] = d_directed
        ygdy8_item['d_duration'] = d_duration
        ygdy8_item['d_content'] = d_content
        ygdy8_item['d_pic'] = d_pic
        ygdy8_item['d_contentpic'] = d_contentpic
        ygdy8_item['d_downurl'] = d_downurl
        ygdy8_item['d_year']=d_year
        ygdy8_item['d_class']=d_class
        yield ygdy8_item


    def parse_donman(self,response):

        ygdy8_item = Ygdy8CrawlItem()
        movieUrl = response.meta['movieUrl']
        d_name = response.meta['d_name']
        d_remarks = response.meta['d_remarks']
        d_title = response.meta['d_title']

        #下面解析电影的其他内容
        ##Zoom>span>img提取图片

        content = response.xpath(".//*[@id='Zoom']")
        content = content[0].xpath("string(.)").extract()[0].strip().replace(r'\n','')
        d_subname = ''

        pattern = re.compile(u'\[([\s\S]*?)\]([\s\S]*)')
        result = re.search(pattern, d_title)
        d_type=''
        d_year=0
        if result is not None:
            d_type = result.groups()
            if d_type > 1:
                d_type = d_type[0].strip()

        d_area = ''
        d_lang = ''

        d_starring = ''
        d_directed = ''
        d_duration = 0
        d_content = content

        #解析图片链接
        pic_list = response.css("#Zoom img::attr(src)").extract()
        if len(pic_list)>1 :
            d_pic = pic_list[0]
            d_contentpic = pic_list[1]
        elif len(pic_list)==1:
            d_pic = pic_list[0]
            d_contentpic=''
        else:
            d_pic = ''
            d_contentpic=''

        d_downurl = response.css("#Zoom a[href^=ftp]::attr(href)").extract()
        d_class=''
        ygdy8_item['movieUrl']=movieUrl
        ygdy8_item['d_name'] = d_name
        ygdy8_item['d_remarks'] = d_remarks
        ygdy8_item['d_subname'] = d_subname
        ygdy8_item['d_type'] = d_type
        ygdy8_item['d_area'] = d_area
        ygdy8_item['d_lang'] = d_lang
        ygdy8_item['d_starring'] = d_starring
        ygdy8_item['d_directed'] = d_directed
        ygdy8_item['d_duration'] = d_duration
        ygdy8_item['d_content'] = d_content
        ygdy8_item['d_pic'] = d_pic
        ygdy8_item['d_contentpic'] = d_contentpic
        ygdy8_item['d_downurl'] = d_downurl
        ygdy8_item['d_year']=d_year
        ygdy8_item['d_class']=d_class
        yield ygdy8_item


