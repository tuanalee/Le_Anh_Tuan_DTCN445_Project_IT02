#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Location {
    char name[50];
    char address[100];
};

struct Trip {
    char tripId[20];
    struct Location departure;
    struct Location destination;
    char dateTime[20];
    int totalSeats;
    int bookedSeats;
};

struct Passenger {
    char name[50];
    char phone[15];
};

struct Ticket {
    char ticketId[20];
    char tripId[20];
    struct Passenger passenger;
    int seatNumber;
    double price;
    int paymentStatus;
};

struct Trip trips[300];
int tripCount = 0;

struct Ticket tickets[1200];
int ticketCount = 0;

int isValidDateTime(const char *dt)
{
    int d, m, y, hh, mm;

    if (sscanf(dt, "%d/%d/%d %d:%d", &d, &m, &y, &hh, &mm) != 5)
        return 0;

    if (y < 2024 || y > 2100) return 0;
    if (m < 1 || m > 12) return 0;
    if (d < 1 || d > 31) return 0;
    if (hh < 0 || hh > 23) return 0;
    if (mm < 0 || mm > 59) return 0;

    return 1;
}

int isValidPhone(const char *phone)
{
    int len = strlen(phone);

    if (len < 10 || len > 15)
        return 0;

    for (int i = 0; i < len; i++)
    {
        if (phone[i] < '0' || phone[i] > '9')
            return 0;
    }

    return 1;
}

void clearInput()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pauseScreen()
{
    printf("\nNhan Enter de quay lai menu...");
    getchar();
}

void inputString(char *label, char *str, int maxLen)
{
    while (1)
    {
        printf("%s", label);

        if (!fgets(str, maxLen, stdin))
            continue;

        str[strcspn(str, "\n")] = '\0';

        if (strlen(str) == 0)
        {
            printf("Khong duoc de trong! Nhap lai.\n");
            continue;
        }

        break;
    }
}

int isTripIdExists(const char *id)
{
    for (int i = 0; i < tripCount; i++)
    {
        if (strcmp(trips[i].tripId, id) == 0)
            return 1;  
    }
    return 0; 
}

void addTrip()
{
    struct Trip t;

    while (1)
{
    inputString("Nhap ma chuyen di: ", t.tripId, sizeof(t.tripId));

    if (isTripIdExists(t.tripId))
    {
        printf("Ma chuyen di da ton tai! Hay nhap ma khac.\n");
        continue;
    }
    break;
}

    inputString("Nhap noi xuat phat: ", t.departure.name, sizeof(t.departure.name));
    inputString("Nhap dia chi noi di: ", t.departure.address, sizeof(t.departure.address));
    inputString("Nhap noi den: ", t.destination.name, sizeof(t.destination.name));
    inputString("Nhap dia chi noi den: ", t.destination.address, sizeof(t.destination.address));

    do
    {
        inputString("Nhap ngay gio (dd/mm/yyyy hh:mm): ", t.dateTime, sizeof(t.dateTime));

        if (!isValidDateTime(t.dateTime))
            printf("Dinh dang khong hop le! Vi du: 25/11/2025 14:30\n");

    } while (!isValidDateTime(t.dateTime));

    printf("Tong so ghe (1-100): ");

    while (scanf("%d", &t.totalSeats) != 1 || t.totalSeats <= 0 || t.totalSeats > 100)
    {
        printf("Khong hop le! Nhap lai: ");
        clearInput();
    }
    clearInput();

    t.bookedSeats = 0;
    trips[tripCount++] = t;

    printf("Them chuyen di thanh cong!\n");
    pauseScreen();
}

void updateTrip()
{
    if (tripCount == 0)
    {
        printf("Chua co chuyen di.\n");
        pauseScreen();
        return;
    }

    char buf[128];
    int idx = -1;

    inputString("Nhap ma chuyen di can cap nhat: ", buf, sizeof(buf));

    for (int i = 0; i < tripCount; i++)
    {
        if (strcmp(trips[i].tripId, buf) == 0)
        {
            idx = i;
            break;
        }
    }

    if (idx == -1)
    {
        printf("Khong tim thay ma chuyen di.\n");
        pauseScreen();
        return;
    }

    printf("Dang cap nhat chuyen di: %s\n", trips[idx].tripId);

    while (1)
    {
        printf("Nhap noi di moi (Enter de bo qua): ");
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\n")] = 0;
        if (strlen(buf) == 0) break;
        strcpy(trips[idx].departure.name, buf);
        break;
    }

    while (1)
    {
        printf("Nhap dia chi noi di moi (Enter de bo qua): ");
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\n")] = 0;

        if (strlen(buf) == 0) break;

        strcpy(trips[idx].departure.address, buf);
        break;
    }

    while (1)
    {
        printf("Nhap noi den moi (Enter de bo qua): ");
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\n")] = 0;
        if (strlen(buf) == 0) break;
        strcpy(trips[idx].destination.name, buf);
        break;
    }

    while (1)
    {
        printf("Nhap dia chi noi den moi (Enter de bo qua): ");
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\n")] = 0;

        if (strlen(buf) == 0) break;

        strcpy(trips[idx].destination.address, buf);
        break;
    }

    while (1)
    {
        printf("Nhap ngay gio moi (dd/mm/yyyy hh:mm) (Enter de bo qua): ");
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\n")] = 0;

        if (strlen(buf) == 0) break;

        if (!isValidDateTime(buf))
        {
            printf("Ngay gio khong hop le! Nhap lai.\n");
            continue;
        }

        strcpy(trips[idx].dateTime, buf);
        break;
    }

    while (1)
    {
        printf("Nhap tong so ghe moi (Enter de bo qua, >= %d, <=100): ", trips[idx].bookedSeats);
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\n")] = 0;

        if (strlen(buf) == 0) break;

        int seats;
        if (sscanf(buf, "%d", &seats) != 1 || seats < trips[idx].bookedSeats || seats > 100)
        {
            printf("So ghe khong hop le! Nhap lai.\n");
            continue;
        }

        trips[idx].totalSeats = seats;
        break;
    }

    printf("Cap nhat chuyen di thanh cong!\n");
    pauseScreen();
}

void bookTicket()
{
    if (tripCount == 0)
    {
        printf("Chua co chuyen di.\n");
        pauseScreen();
        return;
    }

    char tripId[20];
    inputString("Nhap ma chuyen di can dat ve: ", tripId, sizeof(tripId));

    int idx = -1;

    for (int i = 0; i < tripCount; i++)
    {
        if (strcmp(trips[i].tripId, tripId) == 0)
        {
            idx = i;
            break;
        }
    }

    if (idx == -1)
    {
        printf("Khong tim thay chuyen di.\n");
        pauseScreen();
        return;
    }

    int availableSeats = trips[idx].totalSeats - trips[idx].bookedSeats;

    if (availableSeats <= 0)
    {
        printf("Chuyen di da het ghe!\n");
        pauseScreen();
        return;
    }

    struct Ticket t;

    strcpy(t.tripId, tripId);
    t.paymentStatus = 0;

    inputString("Nhap ten hanh khach: ", t.passenger.name, sizeof(t.passenger.name));

    while (1)
    {
        inputString("Nhap so dien thoai: ", t.passenger.phone, sizeof(t.passenger.phone));

        if (!isValidPhone(t.passenger.phone))
        {
            printf("So dien thoai khong hop le! Chi nhap so, 10-15 ky tu.\n");
            continue;
        }
        break;
    }

    while (1)
    {
        char buf[32];
        int seat;

        printf("Nhap so ghe (con %d ghe): ", availableSeats);

        if (!fgets(buf, sizeof(buf), stdin))
            continue;

        if (sscanf(buf, "%d", &seat) != 1 || seat < 1 || seat > trips[idx].totalSeats)
        {
            printf("So ghe khong hop le!\n");
            continue;
        }

        int ok = 1;

        for (int j = 0; j < ticketCount; j++)
        {
            if (strcmp(tickets[j].tripId, tripId) == 0 && tickets[j].seatNumber == seat)
            {
                ok = 0;
                break;
            }
        }

        if (!ok)
        {
            printf("Ghe da duoc dat, chon ghe khac!\n");
            continue;
        }

        t.seatNumber = seat;
        break;
    }

    while (1)
    {
        char buf[32];
        double price;

        printf("Nhap gia ve (>0): ");

        if (!fgets(buf, sizeof(buf), stdin))
            continue;

        if (sscanf(buf, "%lf", &price) != 1 || price <= 0)
        {
            printf("Gia ve khong hop le!\n");
            continue;
        }

        t.price = price;
        break;
    }

    trips[idx].bookedSeats++;

    sprintf(t.ticketId, "T%03d", ticketCount + 1);
    tickets[ticketCount++] = t;

    printf("Dat ve thanh cong! Ma ve: %s | Ghe: %d\n", t.ticketId, t.seatNumber);
    pauseScreen();
}

void checkTicketStatus()
{
    if (ticketCount == 0)
    {
        printf("Chua co ve nao.\n");
        pauseScreen();
        return;
    }

    printf("\n===== TRANG THAI VE =====\n");

    for (int i = 0; i < ticketCount; i++)
    {
        printf("Ma ve: %s\n", tickets[i].ticketId);
        printf("Chuyen di: %s\n", tickets[i].tripId);
        printf("Ghe: %d\n", tickets[i].seatNumber);
        printf("Hanh khach: %s\n", tickets[i].passenger.name);
        printf("SDT: %s\n", tickets[i].passenger.phone);

        const char *status =
            tickets[i].paymentStatus == 1 ? "Da thanh toan" :
            tickets[i].paymentStatus == 2 ? "Da khoa" :
            tickets[i].paymentStatus == -1 ? "Da huy" :
            "Chua thanh toan";

        printf("Trang thai: %s\n", status);
        printf("-----------------------------------\n");
    }

    pauseScreen();
}

void listTrips()
{
    if (tripCount == 0)
    {
        printf("Chua co chuyen di.\n");
        pauseScreen();
        return;
    }

    const int pageSize = 5;
    int totalPages = (tripCount + pageSize - 1) / pageSize;
    int page = 1;
    char choice;

    while (1)
    {
        system("cls");

        printf("===== DANH SACH CHUYEN DI (Trang %d / %d) =====\n\n", page, totalPages);

        printf("+----------+----------------------+----------------------+---------------------+--------+--------+\n");
        printf("| Trip ID  | Noi Di               | Noi Den              | Ngay Gio            | Booked | Total  |\n");
        printf("+----------+----------------------+----------------------+---------------------+--------+--------+\n");

        int start = (page - 1) * pageSize;
        int end = start + pageSize;
        if (end > tripCount) end = tripCount;

        for (int i = start; i < end; i++)
        {
            printf("| %-8s | %-20s | %-20s | %-19s | %-6d | %-6d |\n",
                trips[i].tripId,
                trips[i].departure.name,
                trips[i].destination.name,
                trips[i].dateTime,
                trips[i].bookedSeats,
                trips[i].totalSeats
            );
        }

        printf("+----------+----------------------+----------------------+---------------------+--------+--------+\n");

        printf("\nN: Trang sau | P: Trang truoc | 0: Thoat\n");
        printf("Nhap lua chon: ");

        scanf(" %c", &choice);
        clearInput();

        if (choice == '0')
	    break;
        else if ((choice == 'N' || choice == 'n') && page < totalPages) page++;
        else if ((choice == 'P' || choice == 'p') && page > 1) page--;
    }
}

void paymentTicket()
{
    if (ticketCount == 0)
    {
        printf("Chua co ve nao.\n");
        pauseScreen();
        return;
    }

    char id[20];
    inputString("Nhap ma ve can thanh toan: ", id, sizeof(id));

    int idx = -1;

    for (int i = 0; i < ticketCount; i++)
    {
        if (strcmp(tickets[i].ticketId, id) == 0)
        {
            idx = i;
            break;
        }
    }

    if (idx == -1)
    {
        printf("Khong tim thay ma ve!\n");
        pauseScreen();
        return;
    }

    if (tickets[idx].paymentStatus == 1)
    {
        printf("Ve nay da duoc thanh toan.\n");
        pauseScreen();
        return;
    }

    printf("\n=== THONG TIN VE ===\n");
    printf("Ma ve: %s\n", tickets[idx].ticketId);
    printf("Chuyen di: %s\n", tickets[idx].tripId);
    printf("Hanh khach: %s\n", tickets[idx].passenger.name);
    printf("SDT: %s\n", tickets[idx].passenger.phone);
    printf("Gia ve: %.2f\n", tickets[idx].price);

    printf("\nXac nhan thanh toan? (Y/N): ");

    char c;
    scanf(" %c", &c);
    clearInput();

    if (c == 'Y' || c == 'y')
    {
        tickets[idx].paymentStatus = 1;
        printf("Thanh toan thanh cong!\n");
    }
    else
    {
        printf("Huy thanh toan.\n");
    }

    pauseScreen();
}

void lockOrCancelTicket()
{
    char buf[32];
    char opt;

    while (1)
    {
        printf("Khoa ve (L) | Huy ve (C) | Quay lai (B): ");
        if (!fgets(buf, sizeof(buf), stdin)) continue;

        buf[strcspn(buf, "\n")] = 0;

        if (strlen(buf) == 0) return;

        if (strlen(buf) == 1)
        {
            opt = buf[0];
            break;
        }

        printf("Chi duoc nhap 1 ky tu L / C / B\n");
        pauseScreen();
        return;
    }

    if (opt == 'B' || opt == 'b') return;

    if (!(opt == 'L' || opt == 'l' || opt == 'C' || opt == 'c'))
    {
        printf("Lua chon khong hop le!\n");
        pauseScreen();
        return;
    }

    char ticketId[20];
    inputString("Nhap ma ve: ", ticketId, sizeof(ticketId));

    int idx = -1;

    for (int i = 0; i < ticketCount; i++)
        if (strcmp(tickets[i].ticketId, ticketId) == 0)
        { idx = i; break; }

    if (idx == -1)
    {
        printf("Ma ve khong ton tai!\n");
        pauseScreen();
        return;
    }

    int status = tickets[idx].paymentStatus;

    if (status == -1)
    {
        printf("Ve nay da bi HUY truoc do!\n");
        pauseScreen();
        return;
    }
    if (status == 2)
    {
        printf("Ve nay da bi KHOA truoc do!\n");
        pauseScreen();
        return;
    }

    if (opt == 'L' || opt == 'l')
    {
        if (status == 1)
        {
            printf("Ve da thanh toan, khong the khoa!\n");
            pauseScreen();
            return;
        }

        tickets[idx].paymentStatus = 2;
        printf("Da khoa ve thanh cong!\n");
        pauseScreen();
        return;
    }

    if (opt == 'C' || opt == 'c')
    {
        if (status == 1)
        {
            printf("Ve da thanh toan, khong the huy!\n");
            pauseScreen();
            return;
        }

        for (int i = 0; i < tripCount; i++)
            if (strcmp(tickets[idx].tripId, trips[i].tripId) == 0)
            {
                trips[i].bookedSeats--;
                break;
            }

        tickets[idx].paymentStatus = -1;
        printf("Da huy ve thanh cong!\n");
        pauseScreen();
        return;
    }
}

void revenueStatistics()
{
    int total = ticketCount;
    int paid = 0;
    int unpaid = 0;
    int locked = 0;
    int canceled = 0;
    double revenue = 0;

    for (int i = 0; i < ticketCount; i++)
    {
        if (tickets[i].paymentStatus == 1)
        {
            paid++;
            revenue += tickets[i].price;
        }
        else if (tickets[i].paymentStatus == 0)
            unpaid++;
        else if (tickets[i].paymentStatus == 2)
            locked++;
        else if (tickets[i].paymentStatus == -1)
            canceled++;
    }

    printf("\n===== THONG KE DOANH THU =====\n");
    printf("Tong so ve: %d\n", total);
    printf("Da thanh toan: %d\n", paid);
    printf("Chua thanh toan: %d\n", unpaid);
    printf("Da khoa: %d\n", locked);
    printf("Da huy: %d\n", canceled);
    printf("Tong doanh thu: %.2f VND\n", revenue);

    pauseScreen();
}

int getChoice()
{
    char buf[32];
    int choice;

    while (1)
    {
        printf("Lua chon cua ban: ");

        if (!fgets(buf, sizeof(buf), stdin))
            continue;

        buf[strcspn(buf, "\n")] = 0;

        if (strlen(buf) == 0)
        {
            printf("Khong duoc de trong! Nhap lai.\n");
            continue;
        }

        int isNum = 1;
        for (int i = 0; buf[i] != '\0'; i++)
        {
            if (buf[i] < '0' || buf[i] > '9')
            {
                isNum = 0;
                break;
            }
        }

        if (!isNum)
        {
            printf("Chi duoc nhap so! Nhap lai.\n");
            continue;
        }

        choice = atoi(buf);

        if (choice < 0 || choice > 8)
        {
            printf("Chi duoc nhap 0-8! Nhap lai.\n");
            continue;
        }

        return choice;
    }
}

int main()
{
    int choice;

    printf("CHAO MUNG DEN VOI HE THONG QUAN LY CHUYEN DI!\n");

    tickets[ticketCount++] = (struct Ticket){
        "T001",
        "TR001",
        {"Nguyen Van A", "0912345678"},
        5,
        120000,
        0
    };

    trips[tripCount++] = (struct Trip){
        "TR001",
        {"Ha Noi", "Ben xe My Dinh"},
        {"Hai Phong", "Ben xe Niem Nghia"},
        "28/12/2025 08:30",
        40, 2
    };

    trips[tripCount++] = (struct Trip){
        "TR002",
        {"Da Nang", "Ben xe Trung Tam"},
        {"Hue", "Ben xe Phia Nam"},
        "30/12/2025 14:00",
        50, 1
    };

    trips[tripCount++] = (struct Trip){
        "TR003",
        {"Ho Chi Minh", "Ben xe Mien Dong"},
        {"Vung Tau", "Ben xe Vung Tau"},
        "01/21/2026 07:15",
        45, 0
    };

    while (1)
    {
        printf("\n===== SYSTEM MENU =====\n");
        printf("1. Add trip\n");
        printf("2. Update trip\n");
        printf("3. Book ticket\n");
        printf("4. Check ticket status\n");
        printf("5. Display trip list\n");
        printf("6. Payment\n");
        printf("7. Lock/Cancel ticket\n");
        printf("8. Revenue & statistics\n");
        printf("0. Exit\n");
        printf("=========================\n");

        choice = getChoice();

        switch (choice)
        {
            case 1:
			 system("cls"); 
			 addTrip(); 
			 system("cls"); 
			 break;
			 
            case 2:
			 system("cls"); 
			 updateTrip();
			  system("cls");
			   break;
			   
            case 3: system("cls"); 
			bookTicket(); 
			system("cls"); 
			break;
			
            case 4:
			 system("cls"); 
			checkTicketStatus(); 
			system("cls"); 
			break;
			
            case 5:
			listTrips();
			system("cls");
			 break;
			 
            case 6: system("cls"); 
			paymentTicket();
			 system("cls");
			  break;
			  
            case 7: system("cls"); 
			lockOrCancelTicket(); 
			system("cls"); 
			break;
			
            case 8: 
			system("cls");
			 revenueStatistics();
			  system("cls");
			   break;
			   
            case 0:
			 printf("Thoat chuong trinh...\n"); 
			 return 0;
            default: printf("Chuc nang khong hop le!\n"); 
			break;
        }
    }

    return 0;
}
