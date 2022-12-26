/*==============================================================*/
/* DBMS name:      Sybase SQL Anywhere 12                       */
/* Created on:     2022/11/16 17:29:06                          */
/*==============================================================*/


if exists(select 1 from sys.sysforeignkey where role='FK_COURSE_12N_0_TEACHER') then
    alter table course
       delete foreign key FK_COURSE_12N_0_TEACHER
end if;

if exists(select 1 from sys.sysforeignkey where role='FK_PICK_N2N_0_STU') then
    alter table pick
       delete foreign key FK_PICK_N2N_0_STU
end if;

if exists(select 1 from sys.sysforeignkey where role='FK_PICK_N2N_1_COURSE') then
    alter table pick
       delete foreign key FK_PICK_N2N_1_COURSE
end if;

drop index if exists Stu.Stu_PK;

drop table if exists Stu;

drop index if exists course.12n_0_FK;

drop index if exists course.course_PK;

drop table if exists course;

drop index if exists pick.n2n_0_FK;

drop index if exists pick.n2n_1_FK;

drop index if exists pick.pick_PK;

drop table if exists pick;

drop index if exists teacher.teacher_PK;

drop table if exists teacher;

/*==============================================================*/
/* Table: Stu                                                   */
/*==============================================================*/
create table Stu 
(
   sno                  char(10)                       not null,
   sname                char(10)                       not null,
   sex                  char(2)                        not null,
   major                char(10)                       not null,
   constraint PK_STU primary key (sno)
);

/*==============================================================*/
/* Index: Stu_PK                                                */
/*==============================================================*/
create unique index Stu_PK on Stu (
sno ASC
);

/*==============================================================*/
/* Table: course                                                */
/*==============================================================*/
create table course 
(
   cno                  char(10)                       not null,
   tno                  char(10)                       not null,
   cname                char(10)                       not null,
   credit               integer                        not null,
   "time"               integer                        not null,
   constraint PK_COURSE primary key (cno)
);

/*==============================================================*/
/* Index: course_PK                                             */
/*==============================================================*/
create unique index course_PK on course (
cno ASC
);

/*==============================================================*/
/* Index: 12n_0_FK                                              */
/*==============================================================*/
create index 12n_0_FK on course (
tno ASC
);

/*==============================================================*/
/* Table: pick                                                  */
/*==============================================================*/
create table pick 
(
   cno                  char(10)                       not null,
   sno                  char(10)                       not null,
   grade                integer                        not null,
   constraint PK_PICK primary key (cno, sno)
);

/*==============================================================*/
/* Index: pick_PK                                               */
/*==============================================================*/
create unique index pick_PK on pick (
cno ASC,
sno ASC
);

/*==============================================================*/
/* Index: n2n_1_FK                                              */
/*==============================================================*/
create index n2n_1_FK on pick (
cno ASC
);

/*==============================================================*/
/* Index: n2n_0_FK                                              */
/*==============================================================*/
create index n2n_0_FK on pick (
sno ASC
);

/*==============================================================*/
/* Table: teacher                                               */
/*==============================================================*/
create table teacher 
(
   tno                  char(10)                       not null,
   tname                char(10)                       not null,
   major                char(10)                       not null,
   title                char(10)                       not null,
   age                  integer                        not null,
   constraint PK_TEACHER primary key (tno)
);

/*==============================================================*/
/* Index: teacher_PK                                            */
/*==============================================================*/
create unique index teacher_PK on teacher (
tno ASC
);

alter table course
   add constraint FK_COURSE_12N_0_TEACHER foreign key (tno)
      references teacher (tno)
      on update restrict
      on delete restrict;

alter table pick
   add constraint FK_PICK_N2N_0_STU foreign key (sno)
      references Stu (sno)
      on update restrict
      on delete restrict;

alter table pick
   add constraint FK_PICK_N2N_1_COURSE foreign key (cno)
      references course (cno)
      on update restrict
      on delete restrict;

