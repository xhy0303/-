import requests  # 导入请求模块
import json  # 导入json模块
import csv  # 导入csv模块
import time  # 导入时间模块


def get_country_province():  # 获取全国省份级别流入
    url = 'https://huiyan.baidu.com/migration/cityrank.jsonp?dt=country&id=0&type=move_in&callback=jsonp_1630506921839_9633057'
    response = requests.get(url, timeout=10)  # #发出请求并json化处理
    time.sleep(1)  # 挂起一秒
    r = response.text[3:-1]  # 去头去尾
    data_dict = json.loads(r)  # 字典化
    if data_dict['errmsg'] == 'SUCCESS':
        data_list = data_dict['data']['list']
        with open("全国省份级别流入.csv", "w+", newline="") as csv_file:
            writer = csv.writer(csv_file)
            header = ["province_name", "value"]  # 表头"city_name",
            writer.writerow(header)  # 把表头写入
            for i in range(len(data_list)):
                province_name = data_list[i]['province_name']  # 省份名
                value = data_list[i]['value']
                writer.writerow([province_name, value])  # city_name


# def get_country_city():  # 获取全国城市级别流出
#     url = 'http://huiyan.baidu.com/migration/cityrank.jsonp?dt=country&id=0&type=move_out&date=20200218'
#     response = requests.get(url, timeout=10)  # #发出请求并json化处理
#     time.sleep(1)  # 挂起一秒
#     r = response.text[3:-1]  # 去头去尾
#     data_dict = json.loads(r)  # 字典化
#     if data_dict['errmsg'] == 'SUCCESS':
#         data_list = data_dict['data']['list']
#         with open("全国城市级别流出.csv", "w+", newline="") as csv_file:
#             writer = csv.writer(csv_file)
#             header = ["city_name", "province_name", "value"]  # 表头city_name
#             writer.writerow(header)  # 把表头写入
#             for i in range(len(data_list)):
#                 city_name = data_list[i]['city_name']  # 城市名
#                 province_name = data_list[i]['province_name']  # 省份名
#                 value = data_list[i]['value']
#                 writer.writerow([city_name, province_name, value])  # city_name
#
#
# def generate_url():  # 定义生成不同时期，不同城市，不同迁徙方向，不同层级的请求url
#     date_list = ['20200129', '20200130', '20200131', '20200201', '20200202', '20200203',
#                  '20200204', '20200205', '20200206', '20200207', '20200208', '20200209']  # 初五到初十六时间段
#     directions = ['in', 'out']  # 迁徙方向
#     level_list = ['province', 'city']  # 数据级别
#     city_list = {'北京': '110000', '上海': '310000', '广州': '440100', '深圳': '440300',
#                  '杭州': '330100', '南京': '320100', '天津': '120000', '成都':
#                      '510100', '武汉': '420100', '苏州': '320500'}  # 构造重点10城字典
#     urls = []  # 用来存放url
#     for city_id in list(city_list.values()):  # 城市id
#         for direction in directions:  # 迁入还是迁出
#             for date in date_list:  # 对日期循环
#                 for level in level_list:
#                     url = 'http://huiyan.baidu.com/migration/{}rank.jsonp?dt=country&id={}&type=move_{}&&date={}'.format(
#                         level, city_id, direction, date)  # 请求url
#                     print(url)
#                     urls.append(url)  # 追加进去
#     return urls


if __name__ == "__main__":
    get_country_province()
    # get_country_city()
    # generate_url()
