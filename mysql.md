## MySql知识点整理
### 单表
* DISTINCT 去掉重复数据
* is NULL --- 判断是否为空
* 模糊查询 ---LIKE' %' 
* order by子句排序
    ```
    SELECT * FROM emp ORDER BY deptno ASC, sal DESC; 
    先按照部门排序再按照工资排序
    ```
* 返回行总数
  SELECT COUNT(*) | COUNT(column_name) FROM table_name [WHERE where_definition]
* 返回满足where条件的列和,均值，最大值
  SELECT SUM[AVG][MAX] (column_name) FROM tablename
* having限制分组显示结果
    ```
    SELECT AVG(sal) AS avg_sal, deptno FROM emp GROUP BY depatno HAVING avg_sal<2000; 
    (按照部门分组) 得出的结果为平均工资小于2000的部门号和平均工资(别名用作防止重复计算AVG)
    ```
* USER() : 查询用户
  ```
  SELECT USER() [FROM DUAL]; 查询当前用户@IP地址
  ```
* DATABASE() : 查询当前使用的数据库名称
* SELECT * FROM mysql.user : 查询 数据库.表 就不用手动切换数据库了
#### 1. 字符串
* CHARSET(str) --- 返回字符串的表示规则，如utf-8
* CONCAT(string2, string3, . . .) --- 连接字符串，将多个列拼接为一列
  ```
  SELECT CONCAT(ename, 'job is', job) FROM emp; 把ename和job拼成一列
  ```
* INSTR(string, substring) : 返回substring在string中的位置，没有则返回0
  ```
  SELECT INSTR('hanshunping', 'ping') FROM DUAL 返回值为8 （DUAL为亚元表，做测试表使用）
  ```
* UCASE(string) --- 转换成大写
* LCASE(string) --- 转换成小写
* LEFT|RIGHT(string2, length) --- 从string2中的左边或者右边起取length个字符
* LENGTH(string) --- string长度【按照字节】
* REPLACE(str, search_str, replace_str) --- 在str中用replace_str代替search_str
* STRCMP(string1, string2) --- 逐字符比较两个字符串的大小
* SUBSTRING(str, position [, length]) --- 从str的position开始【从1开始计算】，取length个字符
* LTRIM(string2) | RTRIM(string2) | TRIM() : 去除前端空格或后端空格或把两端都去掉
  ```
  以首字母小写的方式显示所有员工emp表的姓名
  SELECT CONCAT(LCASE(SUBSTRING(ename,1,1)), SUBSTRING(ename,2)) FROM emp;
  ```
#### 2. 取整
* BIN(decimal_number) : 十进制转二进制
* CEILING(num) : 向上取整
* CONV(num, from_base, to_base) : 进制转换
* FORMAT(num, decimal_places) : 保留小数位数
* HEX(decimal_number) : 转十六进制
* LEAST(num, num2, ... ) : 求最小值
* MOD(numberator, denominator) : 求余
* RAND([seed]) --- 范围为[0-1.0] 即返回一个随机浮点数v，若已指定一个整数参数N，则它被用作种子值，用来产生重复序列
  ```
  SELECT RAND() FROM DUAL;
  SELECT RAND(3) FROM DUAL; 结果变成随机数不会再改变，即3被用作种子，重复取值。换种子还会变值并且固定
  ```
#### 3. 时间
* CURRENT_TIMESTAMP() : 当前时间戳（包含年月日，时分秒）
  ```
  INSERT INTO mes VALUES(1, '北京新闻', CURRENT_TIMESTAMP());
  ```
* DATE(datetime) : 返回datetime的日期部分
  ```
  SELECT id, content, DATE(send_time) FROM mes; 结果是显示所有信息，但是只显示日期，去掉时分秒
  ```
* DATE_ADD|SUB(date2, INTERVEL d_value d_type) : 在date2中加上或者减去日期或者时间
  ```
  SELECT * FROM mes WHERE DATE_ADD(send_time, INTERVAL 10 MINUTE) >= NOW();
  显示在10分钟内发布的新闻
  ```
* DATEDIFF(date1, date2) : 两个日期差（结果是天数）
* TIMEDIFF(date1, date2) : 两个时间差（时分秒）
* YEAR|MONTH|DATE(datetime) FROM_UNIXTIME : 年月日
  ```
  SELECT YEAR(NOW()) FROM DUAL;只取年的部分
  ```
* UNIX_TIMESTAMP() : 返回1970-1-1到现在的秒数
  ```
  SELECT UNIX_TIMESTAMP() FROM DUAL;
  ```
* FROM_UNIXTIME() : 把UNIX_TIMESTAMP秒数转成指定格式的日期
  ```
  SELECT FROM_UNIXTIME(1618483438, '%Y-%m-%d %H:%i:%s') FROM DUAL;
  ```
#### 4. 加密
* MD5(str) : 为字符串算出一个MD5 32的字符串，（用户密码）加密
  ```
  INSERT INTO hsp_user VALUES(100, '余川', MD5('bFERPmDy2cc'))；对我的密码进行加密，不让别人看
  查询的时候 SELECT * FROM hsp_user WHERE `name` = '余川' AND pwd = MD5('bFERPmDy2cc')
  ```
* PASSWORD(str) : 为原文密码str 计算并返回密码字符串，通常用于对mysql数据库的用户密码加密，这是mysql默认的密码加密方法
#### 5. 如果-要么-那么
* IF(expr1, expr2, expr3) : 如果expr1为True，则返回expr2否则返回expr3
  ```
  SELECT ename, IF(comn IS NULL, 0.0, comn) FROM emp;
  ```
* IFNULL(expr1, expr2) : 如果expr1不为空NULL，则返回expr1，否则返回expr2
* SELECT CASE WHEN exper1 THEN expr2 WHEN expr3 THEN expr4 ELSE * expr5 END; [类似多重分支]，如果...那么...如果...那么...否则...
  ```
  SELECT ename, (SELECT CASE WHEN job = 'CLERK' THEN '职员' WHEN job = 'MANAGER' THEN '经理' ELSE job END) FROM emp；
  ```
#### 6. 分页 LIMIT
* SELECT . . . LIMIT START, ROWS 从start+1行开始取，取出rows行，start从零开始计算
  ```
  SELECT * FROM emp ORDER BY id LIMIT 0, 3
  ```
  ```

  统计没有获得奖金的雇员数
  SELECT COUNT(*), COUNT(IF(comn IS NULL, 1, NULL)) FROM emp
  显示管理者的总人数（去重）
  SELECT COUNT(DISTINCT mgr) FROM emp;
  显示雇员工资的最大差额
  SELECT MAX(sal)-MIN(sal) FROM emp;
  ```
* 几个语法顺序：GROUP BY -> HAVING -> ORDER BY -> LIMIT
  ```
  统计各个部门的平均工资，并且是大于1000的，按照平均工资从高到低排序，取出前两行记录
  SELECT deptno AVG(sal) AS avg_sal FROM emp GROUP BY deptno HAVING avg_sal>1000 ORDER BY avg_sal DESC LIMIT 0, 2
  ```
---
### 多表查询
```
SELECT * FROM emp, dept WHERE emp.deptno = dept.deptno; 过滤笛卡尔集后返回需要的表,where就是过滤条件
SELECT ename, sal, dname, emp.deptno FROM emp, dept WHERE emp.deptno = dept.deptno; 
SELECT ename, sal, grade FROM emp, salgrade WHERE sal BETWEEN losal AND hisal;
```
#### 1. 自连接
员工和上级是通过一个表的mgr关联，即mgr显示的是上级的员工编号，该上级也在这个表中，这里要显示员工和他的上级的名字，必须用自连接
```
SELECT worker.ename, boss.ename FROM emp worker , emp boss WHERE worker.mgr = boss.empno; 这里一定要把表名取别名
```
#### 2. 子查询
* 单行子查询：显示与SMITH同一部门的所有员工
  ```
  SELECT deptno FROM emp WHERE ename = 'SMITH';把这个语句当作一个子查询
  SELECT * FROM emp WHERE deptno = (SELECT deptno FROM emp WHERE ename = 'SMITH')
  ```
* 多行子查询： 显示和10号部门岗位相同的雇员名字、岗位、工资、部门号，但不包含10号部门的雇员
  ```
  SELECT DISTINCT job FROM emp WHERE deptno = 10;
  SELECT ename, job, sal, deptno FROM emp WHERE job IN(SELECT DISTINCT job FROM emp WHERE deptno = 10) AND deptno != 10;
  ```
* 把子查询的结果当作一个表来使用
  ```
  查询ecshop中各个类别价格最高的商品
  SELECT goods_id, cat_id, goods_name, MAX(shop_price) AS max_price FROM ecs_goods GROUP BY cat_id;
  SELECT cat_id, MAX(shop_price) AS max_price FROM ecs_goods GROUP BY cat_id; 把这个表当成临时表，和原来的表建立连接，显示正确的结果
  SELECT goods_id, ecs_goods.cat_id, goods_name, shop_price FROM(SELECT cat_id, MAX(shop_price)  AS max_price FROM ecs_goods GROUP BY cat_id) temp, ecs_goods WHERE temp.cat_id = ecs_good.cat_id AND temp.max_price = ecs_goods.shp_price
  ```
* ALL&ANY
  ```
  显示工资比30号部门所有员工高的员工姓名、工资和部门号
  SELECT ename, sal, depyno, FROM emp WHERE sal > ALL(SELECT sal FROM emp WHERE deptno = 30)
  ANY就是其中任意一个
  ```
* 多列子查询
  (column1, column2, . . .) = (SELECT column1, column2, . . . FROM . . . )
  ```
  显示与SMITH部门和岗位完全相同的雇员且不含SMITH
  SELECT * FROM emp WHERE (deptno, job) = (SELECT deptno, job FROM emp WHERE ename = 'SMITH') AND ename != 'SMITH'
  表.* 表示将该表所有列都显示
  ```
#### 3. 表复制和去重（蠕虫复制）
INSERT INTO my_tab (id, `name`, sal, job, deptno) SELECT empno, ename, sal, job, deptno FROM emp;
自我复制： INSERT INTO my_tab SELECT * FROM my_tab 常用来测试执行效率
面试题：如何删除掉一张表的重复记录
CEWATE TABLE my_temp LIKE emp;  把emp表结构复制
INSERT INTO my_temp SELECT DISTINCT * FROM my_tab; 把数据复制
其实这里直接把临时表改个名字然后把原表drop掉即可，但这里没有这样做
DELETE FROM my_table; 把原表清掉，然后再复制过来，然后在drop掉临时表
#### 4. 合并查询UNION&UNION ALL
合并多个SELECT语句的结果
SELECT ename, sal, job FROM emp WHERE sal > 2500;
SELECT ename, sal, job FROM emp WHERE job = 'MANAGER'
UNION ALL 将两个查询结果合并，但不会去重 UNION会自动去重
#### 5. 外连接
列出部门名称和部门员工名字和工作，同时要求显示没有员工的部门
SELECT dname, ename, job FROM emp, dept WHERE emp.deptno = dept.deptno ORDER BY dname
左外连接->左边的表完全显示
SELECT `name`, stu.id, grade FROM stu, exam WHERE stu.id = exam.id;
改成左外连接
SELECT `name`, stu.id, grade FROM stu LEFT JOIN exam ON stu.id = exam.id;
#### 6. NOT NULL, UNIQUE, PRIMARY KEY, FOREIGN KEY, CHECK约束
* PRIMARY KEY（主键）
  ```
  CREATE TABLE my_table  (id INT PRIMARY KEY, `name` VARCHAR(32), email VARCHAR(32));
  一张表最大只能有一个主键，但可以是复合主键 
  （CREATE TABLE my_table  (id INT, `name` VARCHAR(32), email VARCHAR(32)，PRIMARY KEY(id, `name`));
  (id和name要同时不相同，同时相同违反约束)
  ```
* UNIQUE （唯一）：column type UNIQUE [NOT NULL]: 该列值不重复
* FOREIGN KEY(外键) ：FOREIGN KEY (column) REFERENCES table_name (primaryKey_name||column) 定义主表和从表关系。
CREATE TABLE my_class (id INT PRIMARY KEY, `name` VARCHAR(32) NOT NULL DEFAULT ' '); 创建主表
CREATE TABLE my_stu (id INT PRIMARY KEY, `name` VARCHAR(32) NOT NULL DEFAULT ' ', class_id INT, FOREIGN KEY (class_id) REFERENCES my_class(id)); 创建从表
1 外键定义在从表上，前提是主键必须定义有主键或者UNIQUE约束
2 表的类型必须是innodb，才支持外键
3 外键字段类型要和主键字段类型一样
4 外键字段的值，必须在主键字段中出现过或者为NULL【前提是外键字段允许为NULL】
5 如果主外键关系形成了，数据就不能随意删除了
DELETE FROM my_class WHERE id = 100 (取决于有没有外键记录指向它)
* CHECK : column type CHECK (check_condition) : 强制行数据必须满足的条件
CREATE TABLE my_table (id INT PRIMARY KEY, `name` VARCHAR(32), sex VARCHAR(6) CHECK (sex IN('man', 'woman')), sal DOUBLE CHECK (sal > 1000 AND sal < 2000));
重点：如果不生效可以暂时用枚举：sex ENUM('男', '女') NOT NULL
#### 7. 自增长-AUTO_INCREMENT
CREATE TABLE my_table (id INT PRIMARY KEY AUTO INCREMENT, email VARCHAR(32) NOT NULL DEFAULT ' ', `name` VARCHAR(32) NOT NULL DEFAULT ' ');
1. INSERT INTO my_table VALUES (NULL, 'jack@qq.com', 'jack'); (这里id从1开始自增长)
2. INSERT INTO my_table (email, `name`) VALUES ('jack@qq.com', 'jack'); (没有赋值，也会自动增长，从1开始)
重点：一般来说自增长和主键配合使用
1. 自增长也可以单独使用，但需要配合UNIQUE
2. 自增长修饰的字段最好为整形
3. 自增长默认从1开始，但也可以改：ALTER table_name AUTO_INCREMENT = 新的开始值
4. 当添加数据时，给自增长字段指定值，则以指定的值为准
#### 8. 索引
CREATE INDEX emptno_index ON emp (empno); (emptno_index是索引名称，后面表示emp表的empno列上创建索引) 查询速度起飞
1. 只对创建了索引的列有效
2. 主键就是一个索引
3. UNIQUE索引
4. 普通索引INDEX（允许列重复）
5. 全文索引（FULLTEXT），适用于MyISAM->一个搜索引擎。 开发中考虑使用：全文搜索Solr和ElasticSearch(ES)
* 添加索引
CREATE [UNIQUE] INDEX index_name ON table_name (col_name [(length)] [ASC|DESC], . . . )
    ```
    CREATE UNIQUE INDEX id_index ON my_table (id)
    ALTER TABLE table_name ADD INDEX [index_name] (index_col_name, ...)
    ALTER TABLE my_table ADD INDEX id_name (id) 
    ```
* 添加主键索引
ALTER TABLE table_name ADD PRIMARY KEY (col_name);
一般在建表确定主键的时候就已经定义了主键所以，如果没有定义，那么就采取下面方法
    ```
    ALTER TABLE my_table ADD PRIMARY KEY (id)
    ```
* 删除索引
DROP INDEX index_name ON table_name;
ALTER  TABLE table_name DROP INDEX index_name;
* 删除主键索引
ALTER TABLE table_name DROP PRIMARY KEY 
* 查询索引
SHOW INDEX(ES) FROM table_name
SHOW KEYS FROM table_name
#### 9. 事务
* 事务的几个基本操作：
1 start transaction -- 开启事务   ||     set autocommit = off
2 savepoint -- 设置保存点
```
eg: SAVEPOINT a 设置保存点a
执行一系列操作后
eg: SAVEPOINT b 设置保存点b
又一系列操作
```
3 rollback to -- 回退事务
发现错误 ROLLBACK TO b
又发现错误，继续回退 ROLLBACK TO a
4 rollback -- 回退全部事务
5 commit -- 提交事务，所有操作生效，不能回退 
```
UPDATE balance SET money = money - 100 WHERE id = 100;
UPDATE balance SET money = money +100 WHERE id = 200;
```
重要：事务依然需要innodb存储引擎
#### 10. 事务的隔离级别
	1 脏读：你还没提交，我这边看到了（两个事务同时在进行）隔离级别为读未提交
	2 不可重复读：你提交了，我还在事务中，我一查可以看到你做的修改和插入（造成了不可重复读和幻读）我的事务已经被你影响了！！！！！我事务中查的前后数据不一致
	3 幻读：同一查询在同一事务中多次进行，由于其他提交事务所做的插入操作，每次返回不同的结果集
事务的隔离级别：定义事务与事务之间的隔离程度
4种隔离级别|脏读|不可重复读|幻读|加锁读| 
:---:|:---:|:---:|:---:|:---:|:---
读未提交-read uncommitted|true|true|true|false
读已提交-read committed|false|true|true|false
可重复读-repeatable read|false|false|true|false
可串行化-serializable|false|false|false|true
* 查看当前mysql的隔离级别
SELECT @@tx_isolation;
* 查看系统的隔离级别
SELECT @@global.tx_isolation;
* 设置隔离级别
SET SESSION TRANSACTION ISOLATION LEVEL READ UNCOMMITTTED
* 设置系统的隔离级别
SET GLOBAL TRANSACTION ISOLATION LEVEL READ UNCOMMITTTED
* 选择某个数据库
use database_name;
#### 11. 事务的四个特性
1 原子性
2 一致性
3 隔离性
4 持久性
#### 12. mysql表类型和存储引擎
表的类型是由存储引擎决定的，主要包括MyISAM, innoDB, Memory等
mysql主要支持六种类型：CSV Memory ARCHIVE MRG_MYISAM MYISAM InnoDB
六种类型又分为两类 1 事务安全型（innoDB），2 非事务安全型（myisam memory）
查看所有的存储引擎：
SHOW ENGINES;
创建表的时候定义存储引擎
CREATE TABLE my_table (id INT, `name` VARCHAR(32)) ENGINE MYISAM
修改存储引擎
ALTER TABLE table_name ENGINE = 存储引擎
#### 13. 视图
把重要的column隐藏，用视图，然后授权给用户
视图是一个虚拟表，内容由查询定义，数据来自基表
视图的四条基本指令：
1 CREATE VIEW 视图名 AS SELECT 语句
CREATE VIEW emp_view AS SELECT empno, ename, job, deptno FROM emp;
2 DESC emp_view 查看视图
SELECT * FROM emp_view
3 ALTER VIEW 视图名 AS SELECT 语句
4 SHOW CREATE VIEW 视图名
5 DROP VIEW 视图名1，视图名2
#### 14. 视图多表查询
CREATE VIEW my_emp AS SELECT empno, ename, dname, grade FROM emp, dept, salgrade WHERE emp.deptno = dept.deptno AND (sal BETWEEN losal AND hisal);
#### 15. 用户管理
创建用户：CREATE USER '用户名' @ '允许登录的位置' IDENTIFIED BY '密码'
删除用户：DROP USER '用户名' @ '允许登录的位置'；
原因：当做项目开发时，可以根据不同的开发人员，赋给相应的操作权限
所以管理人员(root)，根据创建不同的用户，赋给相应的权限
查询用户：SELECT * FROM mysql.user
修改自己的密码：SET PASSWORD = PASSWORD('密码')
修改他人的密码：SET PASSWORD FOR '用户名' @ '登录位置' = PASSWORD('密码')
Mysql8.0语法：ALTER USER 'root' @ 'localhost' INENTIFIED BY '密码'
#### 16. 授权
GRAND 授权列表 ON 库.对象名 TO '用户名' @ '登录位置' [IDENTIFIED BY '密码']（用户存在就修改密码，不存在就创建用户）
GRANT SELECT ON ......
GRANT SELECT, DELETE, CREATE ON ......
GRANT ALL [PRIVILEGES] ON ......
拓展：
1 *.*表示所有数据库的所有对象
2 库.* 表示库里的所有对象
回收用户权限：REVOKE 权限列别 ON 库.列表名 FROM '用户名' @ '登录位置'
刷新指令让权限生效：FLUSH PRIVILEGES
## 面试part
#### 1. 数据库的一二三范式
范式是一种设计标准，避免数据冗余和异常
1NF：每个字段不能再拆分，姓名不能拆成姓和名，地址不能拆成省市区
2NF：每个字段依赖主键，而不是部分依赖
3NF：非主键字段互不依赖，可以单独存放在另一个表中
#### 2. 索引类型---唯一，主键，聚集cluster
实现方式：B+树、散列索引、位图索引
#### 3. 聚集索引和非聚集的区别
![](2023-03-02-11-19-17.png)
* 聚集索引---数据按照索引顺序存储，检索效率高，但数据更新影响大。主键索引就是聚集索引，叶子节点存放数据
* 非聚集索引表示数据和索引分开存储，叶子节点存放的是地址。指向数据存储位置，数据更新影响小
* 辅助索引，叶子节点存放主键id，再从id，找到data
> 主键索引就是聚集索引
#### 4. 唯一和主键索引的区别
1. 主键可以包含多个字段，联合索引
2. 主键可以作为外键
3. 主键不可为空
4. 主键只能有一个
#### 5. innodb和myisam的特点和区别
* innodb支持事务，实现了隔离级别，提供行级锁和外键约束，设计目的就是处理大容量数据库系统。mysql运行时innodb会在内存建缓冲区，用于缓冲数据和索引。不支持FULLTEXT类型的索引，没有保存表的行数，当SELECT COUNT* FROM TABLE进行全表扫描。锁的粒度更小，写操作不会锁定全表，所以在并发较高时，innodb会提升效率。但行级锁不是绝对的，如果执行语句不能确定扫描范围，一样会锁全表
* myisam是默认引擎，没有事务支持，不支持行级锁和外键，所以INSERT或UPDATE锁全表，效率偏低。但myisam存储了表的行数。如果读远远多于写且不需要事务支持，可以用myisam
* 大尺寸数据建议innodb，支持事务处理和故障恢复，innodb利用日志恢复。主键查询也快。大批INSERT在myisam会快，UPDATE在innodb会快，尤其是在并发时候
#### 6. MVCC 、 next-key锁解决欢幻读
MVCC---多版本并发：在快照读下，给每个数据行添加两个隐藏的列，记录该行的创建和删除版本号，事务在查询数据时只能看到在自己版本之前创建且未被删除的数据
next-key锁：在当前读情况下，锁定索引上的记录，还锁定索引上相邻记录的间隙，所以有时候称为间隙锁。防止其他事务在间隙中插入新的记录
#### 7. MVCC底层
1. undo log---日志版本链。把每次修改的数据保存一份，每条数据基础上加两个隐藏列，trx_id+roll_pointer，分别存储当前事务ID和上一个版本的数据地址（就是从哪复制过来的）
>如果记录被删除，数据也不会真的被删除，被当作update操作，复制一份数据过来，然后更新trx_id位当前事务id。然后更新记录的头信息为删除状态
2. read view---一致性视图。可重复读级别开启事务，生成一致性视图，提交时前都不会变更。视图有未提交事务数组和已提交最大事务id构成
> [200,300,400] 401
> 括号内事务id不可见，当前我操作的最大的事务id是401，>=401也不可见，是在我之后生成的事务id
#### 8. 快照读-select，实现可重复读的隔离级别，读到的可能不是最新数据
#### 9. 当前读-update,delete, insert,lock，读最新数据，就是加了锁的增删改查
#### 10. MVCC有没有解决幻读的问题？
快照读的情况下，innodb通过MCVV解决了幻读问题
但是当前读情况下，innodb没解决，因为每次读的都是最新数据，只能使用间隙锁
#### 11. 一条SQL的整个执行过程
连接器
---缓存(8.0后不使用)
---分析器(词法语法分析，生成解析树)
---优化器(决定使用哪个索引)
---执行器(调用索引存储引擎API，执行解析后的sql，并返回结果给客户端)
---写undo log
---写redo log(备份)
---写bin log做主从同步
---提交事务
---刷redo log进磁盘
---刷bin log进磁盘(二阶段提交防止事务不一致)
#### 12. 主从复制
* 读写分离，主写从读
* 数据备份
* 高可用
* 架构扩展
1. 异步复制---主节点将数据写入binary log，提交事务，就立即返回客户端，不管有没有同步到从节点
2. 半同步复制---增加等待从节点成功提交事务的逻辑，只要有一个从节点提交，就返回客户端
3. 全同步---等所有节点
4. GTID---解决阶段数据同步丢失的问题。主节点更新binary log之前会产生一个GTID，一同保存到binary log里。当从节点SQL线程读取 relay log 时，提取GTID，如果发现本地已有，说明已经执行过，跳过。其实就是个标志位，无语
#### 13. mysql哪些用法会导致索引失效，导致全表查询了
* 使用 or ---必须or前后的字段都建立了索引
* 使用 like --- 除了like 'xxx%'
* 联合索引 --- 没有联合索引顺序查询，索引abc，where ac bc，不满足最左前缀匹配
* 索引列有运算符 --- ! = not in，主键可以，直接使用索引过滤，非主键要查到然后做比较
* 索引列使用了函数 --- cast
* 隐式类型转换
#### 14. Explain关键字
模拟优化器，查看执行计划信息，分析查询语句或表结构的性能瓶颈，看看有没有走索引优化
#### 15, 关系/非关系型数据库
* 关系：事务一致性，贴近逻辑，使用方便支持复杂查询，便于维护(数据冗余，不一致等)。读写性能差，表结构固定，高并发读写需求，海量数据的高效读写
* 非关系：kv存储，分布式，数据之间无耦合，容易扩展，读写性能高，一般不支持事务(锁，Lua脚本)
#### 16. 数据库连接池---避免数据库频繁连接和销毁，提高效率。线程用完之后还给连接池
#### 17. 数据库连接消耗时间
1. 三次握手
2. 服务器的连接认证
3. 关闭连接的资源回收
4. 四次挥手
#### 18. 数据的锁的种类，加锁的方式
锁出现的目的是处理并发问题---乐观锁，悲观锁，时间戳
数据库的角度分为三种---排他锁，共享锁，更新锁
从程序员的角度---悲观锁，乐观锁
#### 18. union 和 join的区别
* join 是把条件相同的记录交联，产生记录集，union是两个记录及合并
* union 会过滤重复数据，合并是根据行合并，union不会去重
* join 是关联运算的，两表有关系。根据某一列进行笛卡尔运算和条件过滤
#### 19. 常用指令
* 启动和退出
```
mysql -h ip -u root -p -P 3306
mysql -h 127.0.0.1 -u root -p -P 3306
```
* 查看数据库
create database if not exists dbname
* 选择数据库
use database_name;
* 查看数据库的数据表
show tables
* 删除数据库
drop database if exists dbname 
* 创建一个简单的数据库表
字段 类型(长度) 属性 索引
```
create table if not exist 表名(id int unsignd auto_increment primary key, name varchar(255) not null) engine = innodb default charset = utf8;
```
* 添加数据
```
INSERT INTO table_name (column1, column2, column3 . .) VALUES(value1, value2, value3 . .);
```
* 修改数据
update table set col = 'val1', col = 'val2' where condition
* 删除数据
delete from table where condition
* 创建新普通用户
grant 权限 on 库名.表名 to '用户名'@'主机名' identified by '密码'
* 查询所有用户
select user, host from mysql.user
* 删除普通用户
drop user '用户名'@'主机名'
* 修改root密码
set password = password('新密码')
* root用户修改普通用户密码
set password for '用户名'@'主机名' = password('新密码')
* 授权
grant 权限 on 库名.表名 to '用户名'@'主机名' identified by '密码'
grant select, insert, update, selete, on cendxia.user to '用户名'@'主机名' indentified by '密码'
* 查看权限
show grants for '用户名'@'主机名'
* 收回权限
revoke 权限 on 库名.表名 from '用户名'@'主机名'
* 备份
mysqldump -u root -p 数据库名 > 要保存的位置
* 还原数据
mysql -u yser -p dbname < filename.sql
* 建表引擎
myisam -- 读取速度快，不支持事务
innodb -- 读取速度慢，支持事务，事务回滚
* 一些常用属性
  * unsignd 无符号属性
  * auto_increment 自增，一般用在id
  * zerofill 零填充
* 字符串类型
char 定长字符串(0-255)
carchar 变长字符串 5.0以前(0-255), 以后(0-65535)
* 查看表结构
desc 表名
* 查看建表语句
show create table 表名
* 修改表名
alter table 原表名 rename to 新表名
* 修改字段的数据类型
alter table 表名 modify 字段名 数据类型 属性 索引
alter table testalter_tbk modify c char(10)
* 修改字段名
alter table 表名 change 原字段名 新字段名 数据类型 属性 索引
* 增加字段
alter table 表名 add 字段名 数据类型 属性 索引
-- [first|after 字段名]
-- (first 在最前面添加字段， after 字段名 在某字段后面添加)
* 删除字段
alter table 表名 frop 字段名
* 修改字段的排列位置
alter table 表名 modify 字段名 数据类型 属性 索引 after 字段名
* 修改表引擎
ALTER TABLE 表名 ENGINE = 存储引擎
#### 20. 不使用外键的原因
每次做一次修改，比如插入订单，就要去查询用户表和商品表是否存在相应的数据，很难受
1. 触发查询操作，额外的性能消耗
2. 查询的时候，会触发内部锁，可能会造成高并发死锁
3. 用了外键就意味着把所有数据一致交给数据库处理，以后分库分表直接心态爆炸
4. 不用外键刷数据方便，批量导入
5. 防止被开除
解决方案：采用业务逻辑关联模拟外键，不需要数据库层面的强耦合，可以在业务层面采用各种方案。高并发场景下可以先更新部分表，再由消息驱动等方式来进行其他数据补偿