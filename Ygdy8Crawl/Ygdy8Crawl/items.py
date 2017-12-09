# -*- coding: utf-8 -*-

# Define here the models for your scraped items
#
# See documentation in:
# http://doc.scrapy.org/en/latest/topics/items.html

import scrapy


class Ygdy8CrawlItem(scrapy.Item):
    # define the fields for your item here like:
    movieUrl = scrapy.Field()#电影链接
    d_title = scrapy.Field()#电影标题
    d_name = scrapy.Field()#电影名称
    d_subname = scrapy.Field()#副标题
    d_type = scrapy.Field()

    d_class = scrapy.Field()

    d_area = scrapy.Field()
    d_lang = scrapy.Field()
    d_remarks = scrapy.Field()
    d_year = scrapy.Field()
    d_starring = scrapy.Field()
    d_directed = scrapy.Field()
    d_duration=scrapy.Field()

    d_pic = scrapy.Field()
    d_content = scrapy.Field()
    d_contentpic = scrapy.Field()

    d_downurl = scrapy.Field()


