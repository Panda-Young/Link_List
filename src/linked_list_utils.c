/*
 * @Descripttion:
 * @version: 0.1.0
 * @Author: PandaYoung
 * @Date: 2022-04-10 02:24:32
 */
#include "../inc/list.h"

nodeptr_t add_endnode(nodeptr_t end)
{
    if (end == NULL) {
        printf("end node is null.\n");
        return NULL;
    }
    nodeptr_t newnode = (nodeptr_t)malloc(sizeof(node_t));
    if (newnode == NULL) {
        perror("malloc failed: ");
        return NULL;
    }
    end->next = newnode;
    newnode->next = NULL;
    return newnode;
}

nodeptr_t add_beginnode(nodeptr_t head)
{
    if (head == NULL) {
        printf("head node is null.\n");
        return NULL;
    }
    nodeptr_t newnode = (nodeptr_t)malloc(sizeof(node_t));
    if (newnode == NULL) {
        perror("malloc failed: ");
        return NULL;
    }
    newnode->next = head;
    return newnode;
}

nodeptr_t add_randomnode(nodeptr_t head, nodeptr_t current)
{
    if (head == NULL || current == NULL) {
        printf("head or current node is null.\n");
        return NULL;
    }
    nodeptr_t newnode = (nodeptr_t)malloc(sizeof(node_t));
    if (newnode == NULL) {
        perror("malloc failed: ");
        return NULL;
    }

    nodeptr_t tmp = NULL;
#if ANOTHER_WAY // 任意节点前
    for (tmp = head; tmp->next != current; tmp = tmp->next);
    tmp->next = newnode;
    newnode->next = current;
#else // 任意节点后
    tmp = current->next;
    newnode->next = tmp;
    current->next = newnode;
#endif
    return newnode;
}

/**
 * @description: reverse the list
 * @param {nodeptr_t} head
 * @return {nodeptr_t}
 */
nodeptr_t reverse_list(nodeptr_t head)
{
    if (head == NULL || head->next == NULL) {
        return head;
    }
#if ANOTHER_WAY // 递归法
    nodeptr_t Pnext = head->next;
    nodeptr_t Ptemp = reverse_list(Pnext);
    Pnext->next = head;
    head->next = NULL;
    return Ptemp;
#else // 迭代法
    nodeptr_t Ptemp = NULL;
    nodeptr_t Pcurr = head;
    nodeptr_t Pnext;
    while (Pcurr != NULL) {
        Pnext = Pcurr->next;
        Pcurr->next = Ptemp;
        Ptemp = Pcurr;
        Pcurr = Pnext;
    }
    head = Ptemp;
#endif
    return head;
}

/**
 * @Descripttion: save node info to file
 * @param {nodeptr_t} tmp
 * @param {int} mode
 *              0   将当前至结尾所有节点信息写入清空的文件，一般用于头节点
 *              1   将当前节点信息追加至文件末尾
 * @return {int}
 */
int save_to_file(nodeptr_t tmp, bool mode)
{
    FILE *fp = NULL;
    if (mode == 0) {
        if ((fp = fopen(FILE_NAME, "w+")) == NULL) {
            perror("open file failed: ");
            return EFOPEN;
        }
        while(tmp != NULL) {
            fprintf(fp, "%s\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", tmp->data.name, tmp->data.stu_id,
                        tmp->data.score[0], tmp->data.score[1], tmp->data.score[2],
                        tmp->data.stu_age, tmp->data.stu_sex);
            tmp = tmp->next;
        }
    } else if (mode == 1){
        if ((fp = fopen(FILE_NAME, "a+")) == NULL) {
            perror("open file failed: ");
            return EFOPEN;
        }
        fprintf(fp, "%s\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", tmp->data.name, tmp->data.stu_id,
            tmp->data.score[0], tmp->data.score[1], tmp->data.score[2],
            tmp->data.stu_age, tmp->data.stu_sex);
    } else {
        return EMODE;
    }

    fclose(fp);
    return EOK;
}

/**
 * @Descripttion: creat new file to save infomation
 * @param {nodeptr_t} head
 * @param {unsigned char} *confirm_code
 * @return {int}
 */
int create_file(nodeptr_t head, unsigned char *confirm_code)
{
    if (head == NULL) {
        return EALLOC;
    }
    if(*confirm_code == 'y' || *confirm_code == 'Y') {
        nodeptr_t current = head;
        while (1) {
            printf("Input Name, ID, Ch & Math & Eng scor, Age & Sex in proper order.\n");
            scanf("%s %d %d %d %d %d %d", current->data.name, &current->data.stu_id,
                    &current->data.score[0], &current->data.score[1], &current->data.score[2],
                    &current->data.stu_age, &current->data.stu_sex);

            printf("Whether continue to add info? Press 'Y' or any other key to End!\n");
            if (scanf("%c", confirm_code) && *confirm_code == 10) {
                scanf("%c", confirm_code);
            }
            if (*confirm_code == 'y' || *confirm_code == 'Y') {
                current = add_endnode(current);
            } else {
                break;
            }
        }
        save_to_file(head, 0);
    } else {
        printf_yellow("Welcome to Use next time!\n");
        *confirm_code = 0;
    }
    return EOK;
}

/**
 * @description: read information from file
 * @param {nodeptr_t} head
 * @param {bool} *any_info
 * @return {int}
 */
int read_file(nodeptr_t head)
{
    FILE *fp = NULL;
    if ((fp = fopen(FILE_NAME, "r")) == NULL) {
        perror("open file failed: ");
        return EFOPEN;
    }

    int node_num      = 0;
    nodeptr_t current = head;
    while (fscanf(fp, "%s %d %d %d %d %d %d", current->data.name, &current->data.stu_id,
            &current->data.score[0], &current->data.score[1], &current->data.score[2],
            &current->data.stu_age, &current->data.stu_sex) == NUM_ELEMENT) {
        node_num++;
        if(fgetc(fp) == 10 && fgetc(fp) == EOF) {
            break;
        } else {
            fseek(fp, -1, SEEK_CUR);
            if ((current = add_endnode(current)) ==  NULL) {
                printf ("add new link node error\n");
                return EALLOC;
            }
        }
    }
    fclose(fp);
    return node_num;
}

/**
 * @Descripttion: View all information in the file
 * @param {nodeptr_t} head
 * @param {unsigned char} *confirm_code
 * @return {int}
 */
int view_info(nodeptr_t head, unsigned char *confirm_code)
{
    if (head == NULL) {
        printf("malloc failed");
    }

    int node_num = read_file(head);

    if (node_num > 0) {
        printf_green("Name\t\tID\t\tChinese\t\tMath\t\tEnglish\t\tAge\t\tSex\n");
        for (nodeptr_t current = head; current != NULL; current = current->next) {
            printf_green("%s\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", current->data.name,
                current->data.stu_id, current->data.score[0], current->data.score[1],
                current->data.score[2], current->data.stu_age, current->data.stu_sex);
        }
    } else if (node_num == 0) {
        printf("The content of the file is empty! Press 'Y' to add info or any other key to Exit!\n");
        if (scanf("%c", confirm_code) && *confirm_code == 10) {
            scanf("%c", confirm_code);
        }
        create_file(head, confirm_code);
    } else {
        printf("read file error, result %d", node_num);
    }

    return EOK;
}

/**
 * @Descripttion: add new information to the system
 * @param {nodeptr_t} head
 * @return {int}
 */
int add_info(nodeptr_t head, unsigned char *confirm_code)
{
    if (head == NULL) {
        printf("malloc failed");
    }

    while (1) {
        nodeptr_t current = head;
        printf("Input Name, ID, Ch & Math & Eng scor, Age & Sex in proper order.\n");
        scanf("%s %d %d %d %d %d %d", current->data.name, &current->data.stu_id,
                &current->data.score[0], &current->data.score[1], &current->data.score[2],
                &current->data.stu_age, &current->data.stu_sex);
        save_to_file(current, 1);

        printf("Whether continue to add info? Press 'Y' or any other key to End!\n");
        if (scanf("%c", confirm_code) && *confirm_code == 10) {
            scanf("%c", confirm_code);
        }
        if (*confirm_code == 'y' || *confirm_code == 'Y') {
#if ANOTHER_WAY
            head = add_beginnode(head);
#elif ANOTHER_WAY2
            current = add_randomnode(head, head);
#else
            current = add_endnode(current);
#endif
        } else {
            break;
        }
    }
    return EOK;
}

/**
 * @description: Delete someone's information
 * @param {nodeptr_t} head
 * @return {int}
 */
int delete_info(nodeptr_t head)
{
    unsigned int stu_id;
    printf("Please input student id you want to delete.\n");
    scanf("%d", &stu_id);

    nodeptr_t current = head;
    for (; current != NULL; current = current->next) {
        if (current->data.stu_id == stu_id) {
            nodeptr_t Ppre = head;
            for (; Ppre->next != current; Ppre = Ppre->next);
            Ppre->next = current->next;
            free(current);
            current = NULL;
            break;
        }
    }
    save_to_file(head, 0);

    return EOK;
}

/**
 * @description: find somebody's information
 * @param {nodeptr_t} head
 * @return {int}
 */
int find_info(nodeptr_t head)
{
    unsigned int stu_id;
    printf("Please input student id you want to find.\n");
    scanf("%d", &stu_id);

    nodeptr_t current = head;
    for (; current != NULL; current = current->next) {
        if (current->data.stu_id == stu_id) {
            printf_light_blue("Name\t\tID\t\tChinese\t\tMath\t\tEnglish\t\tAge\t\tSex\n");
            printf_light_blue("%s\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
                current->data.name, current->data.stu_id,
                current->data.score[0], current->data.score[1], current->data.score[2],
                current->data.stu_age, current->data.stu_sex);
            break;
        }
    }
    if (current == NULL) {
        printf("No information about the student was found\n");
    }
    return EOK;
}

/**
 * @description: sort information by students's score
 * @param {nodeptr_t} head
 * @return {int}
 */
int sort_info(nodeptr_t head)
{
    if (head->next != NULL) {
#if ANOTHER_WAY
        nodeptr_t end = NULL;
        for(end = head; end->next != NULL; end = end->next);
        quick_sort(head, end);
#else
        bubble_sort(head);
#endif
#if DEBUG_SORT_MSG
        printf_light_blue("Name\t\tID\t\tChinese\t\tMath\t\tEnglish\t\tAge\t\tSex\n");
        for (nodeptr_t current = head; current != NULL; current = current->next) {
            printf_light_blue("%s\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
                current->data.name, current->data.stu_id,
                current->data.score[0], current->data.score[1], current->data.score[2],
                current->data.stu_age, current->data.stu_sex);
        }
#endif
        save_to_file(head, 0);
    }
    return EOK;
}
