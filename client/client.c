#include <stdio.h>
#include <stdlib.h>
#include <usrinc/atmi.h>
#include <usrinc/fbuf.h>
#include <usrinc/tx.h>
#include "../fdl/info_fdl.h"

#define FAIL -1
#define SUCCESS 0

#define FLUSH while((getchar()) != '\n')

int check_account(char *account)
{
    char *sendbuf;
    char *recvbuf;

    long olen;
    int ret;

    if ((sendbuf = tpalloc("STRING", NULL, 0)) == NULL)
    {
        fprintf(stderr,"Error allocation check buffer\n");
        printf("%s\n",tpstrerror(tperrno));
        return FAIL;
    }
    if ((recvbuf = tpalloc("STRING", NULL, 0)) == NULL)
    {
        fprintf(stderr,"Error allocation buffer\n");
        printf("%s\n",tpstrerror(tperrno));
        return FAIL;
    }

    printf("Welcome to the bank!\n");
    while(1)
    {

        printf("Account: ");
        scanf("%s", account);
        FLUSH;

        strcpy(sendbuf, account);

        if (tpcall("CHECKACCOUNT", (char *)sendbuf, 0, &recvbuf, &olen, TPNOFLAGS) == -1)
        {
            // fprintf(stderr,"Can’t send request to service CHECKACCOUNT->%s !\n",
            //         tpstrerror(tperrno));

            printf("Server: %s\n", recvbuf);
            continue;
        }
        break;
    }

    tpfree(sendbuf);
    tpfree(recvbuf);
    return SUCCESS;
}

int transfer(char *account)
{
    FBUF *sendbuf;
    char *recvbuf;
    char *from = account;
    char to[50] ={0};
    long olen;
    int amount;
    int ret;


    printf("Transfer to: ");
    scanf("%s",to);
    FLUSH;

    printf("amount: ");
    scanf("%d",&amount);
    FLUSH;

    if ((sendbuf = tpalloc("FIELD", NULL, 0)) == NULL)
    {
        fprintf(stderr,"Error allocation send buffer\n");
        printf("%s\n",tpstrerror(tperrno));
        return FAIL;
    }
    if ((recvbuf = tpalloc("STRING", NULL, 0)) == NULL)
    {
        fprintf(stderr,"Error allocation buffer\n");
        printf("%s\n",tpstrerror(tperrno));
        return FAIL;
    }
    

    ret=tx_set_transaction_timeout( 5 );
    if (ret<0) 
    {
        return FAIL;
    }

    ret=tx_begin();
    if (ret<0)
    {
        return FAIL;
    }
    
    fbput(sendbuf,AMOUNT,&amount,0);
    fbput(sendbuf,FROM,from,0);
    fbput(sendbuf,TO,to,0);

    ret=tpcall("WITHDRAW", (char *)sendbuf, 0, &recvbuf, &olen, TPNOFLAGS);
    if (ret == -1)
    {
        fprintf(stderr,"Can’t send request to service WITHDRAW->%s !\n",
                    tpstrerror(tperrno));
        printf("Server: %s\n",recvbuf);
        tx_rollback();
        tpfree(sendbuf);
        tpfree(recvbuf);
        return FAIL;
    }


    // fbput(sendbuf,AMOUNT,&amount,0);
    // fbput(sendbuf,FROM,from,0);
    // fbput(sendbuf,TO,to,0);

    ret=tpcall("DEPOSIT", (char *)sendbuf, 0, &recvbuf, &olen, TPNOFLAGS);
    if (ret == -1)
    {
        fprintf(stderr,"Can’t send request to service DEPOSIT->%s !\n",
                    tpstrerror(tperrno));
        printf("Server: %s\n",recvbuf);
        tx_rollback();
        tpfree(sendbuf);
        tpfree(recvbuf);
        return FAIL;
    }

    tx_commit();
    tpfree(sendbuf);
    tpfree(recvbuf);

    return SUCCESS;

}

int main(int argc, char *argv[])
{
    int n=0;
    int amount;
    char account[50] ={0};
    char to[50] ={0};
    long olen;

    int ret;

    if (argc != 1)
    {
        fprintf(stderr, "Usage: $ %s \n", argv[0]);
        exit(1);
    }

    if (tpstart((TPSTART_T *)NULL) == -1)
    {
        fprintf(stderr,"Tpstart failed\n");
        exit(1);
    }

    ret=check_account(account);
    if(ret < 0)
    {

    }

    while(1)
    {
        printf("\nSelect your operation.\n\n");
        printf("0. Exit\n");
        printf("1. TRANSFER \n\n: ");
        // printf("2.\n");
        scanf("%d", &n);
        FLUSH;
        
        switch (n)
        {
        case 0:
            break;

        case 1:
            ret = transfer(account);
            if(ret==FAIL)
            {
                printf("Transfer operation failed.\n");
            }
            break;
        
        default:
            printf("Invalid operation.\n");
            continue;
        }
        break;
    }

    tpend();

}