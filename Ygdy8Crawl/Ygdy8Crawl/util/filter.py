#coding:utf-8
import hashlib

from scrapy.dupefilters import RFPDupeFilter
from scrapy.utils.url import canonicalize_url


class URLSha1Filter(RFPDupeFilter):
      """根据urlsha1过滤"""
      def __init__(self, path=None, debug=False):
        self.urls_seen = set()
        RFPDupeFilter.__init__(self, path)

      def request_seen(self, request):
        fp = hashlib.sha1()
        fp.update(canonicalize_url(request.url))
        url_sha1 = fp.hexdigest()
        if url_sha1 in self.urls_seen:
            return True
        else:
            self.urls_seen.add(url_sha1)