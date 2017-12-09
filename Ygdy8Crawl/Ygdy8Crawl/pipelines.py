# -*- coding: utf-8 -*-

# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: http://doc.scrapy.org/en/latest/topics/item-pipeline.html
import pymongo


class Ygdy8CrawlPipeline(object):
    def __init__(self, mongo_uri, mongo_db):
        self.mongo_uri = mongo_uri
        self.mongo_db = mongo_db


    @classmethod
    def from_crawler(cls, crawler):
        return cls(
            mongo_uri=crawler.settings.get('MONGO_URI'),
            mongo_db=crawler.settings.get('MONGO_DATABASE', 'ygdy8'),
        )

    def open_spider(self, spider):
        self.client = pymongo.MongoClient(self.mongo_uri)
        self.db = self.client[self.mongo_db]

    def close_spider(self, spider):
        self.client.close()
    def process_item(self, item, spider):
        if 'gndy'in item['movieUrl']:
            self.db.movie2.insert(dict(item))
        elif 'tv' in item['movieUrl']:
            self.db.tv.insert(dict(item))
        elif 'zongyi'in item['movieUrl']:
            self.db.zongyi.insert(dict(item))
        elif 'dongman'in item['movieUrl']:
            self.db.dongman.insert(dict(item))
        else:
            self.db.movie2.insert(dict(item))


if __name__=='__main__':
    pass