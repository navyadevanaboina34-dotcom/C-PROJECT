#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct LandRecord
{
    int plotID;
    char ownerName[50];
    char address[100];
    float landArea;
    double landValue;
    char ownershipDate[20];
};
void addRecord();
void displayRecords();
void searchRecord();
void modifyRecord();
void deleteRecord();
int main() 
{
    int choice;
    while (1) 
    {
        printf("\n====== LAND REGISTRATION SYSTEM ======\n");
        printf("1. Add New Land Record\n");
        printf("2. Display All Land Records\n");
        printf("3. Search Land by Plot ID\n");
        printf("4. Modify Land Ownership\n");
        printf("5. Delete Land Record\n");
        printf("6. Exit\n");
        printf("=====================================\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) 
        {
             printf("\nInvalid input! Please enter a number.\n");
             while (getchar() != '\n'); 
            continue; 
        }
        switch (choice) 
        {
            case 1:
                addRecord();
                break;
            case 2:
                displayRecords();
                break;
            case 3:
                searchRecord();
                break;
            case 4:
                modifyRecord();
                break;
            case 5:
                deleteRecord();
                break;
            case 6:
                printf("\nExiting program. Thank you!\n");
                exit(0);
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }
}
void addRecord() {
    FILE *fp;
    struct LandRecord record;
    fp = fopen("land_record.txt", "a");
    if (fp == NULL) 
    {
        printf("Error opening file!\n");
        return;
    }
    printf("\nEnter Plot ID: ");
    scanf("%d", &record.plotID);
    getchar();
    printf("Enter Owner Name: ");
    fgets(record.ownerName, 50, stdin);
    record.ownerName[strcspn(record.ownerName,"\n")] = '\0';
    printf("Enter Address: ");
    fgets(record.address, 100, stdin);
    record.address[strcspn(record.address, "\n")] = '\0';
    printf("Enter Land Area (in sq.ft): ");
    scanf("%f", &record.landArea);
    printf("Enter Land Value: ");
    scanf("%lf", &record.landValue);
    getchar();
    printf("Enter Ownership Date (DD-MM-YYYY): ");
    fgets(record.ownershipDate, 20, stdin);
    record.ownershipDate[strcspn(record.ownershipDate, "\n")] = '\0';
    fprintf(fp, "%d,%s,%.2f,%.2lf,%s\n",
            record.plotID,record.ownerName, record.address,
            record.landArea, record.landValue, record.ownershipDate);
    fclose(fp);
    printf("\nRecord added successfully!\n");
}
void displayRecords() 
{
    FILE *fp;
    struct LandRecord record;
    fp = fopen("land_record.txt", "r");
    if (fp == NULL) {
        printf("\nNo records found!\n");
        return;
    }
    printf("\n================ LAND RECORDS ================\n");
    printf("%-10s %-20s %-12s %-12s %-12s %-20s\n",
           "PlotID", "Owner Name", "Area(sqft)", "Value", "Date", "Address");
    printf("--------------------------------------------------------------------------\n");
    while (fscanf(fp, "%d,%49[^,],%99[^,],%f,%lf,%19[^\n]\n",
                  &record.plotID, record.ownerName, record.address,
                  &record.landArea, &record.landValue, record.ownershipDate) == 6) 
    {
        printf("%-10d %-20s %-12.2f %-12.2lf %-12s %-20s\n",
               record.plotID, record.ownerName, record.landArea,
               record.landValue, record.ownershipDate, record.address);
    }
    fclose(fp);
}
void searchRecord() 
{
    FILE *fp;
    struct LandRecord record;
    int id, found = 0;
    fp = fopen("land_record.txt", "r");
    if (fp == NULL) 
    {
        printf("\nNo records found!\n");
        return;
    }
    printf("\nEnter Plot ID to search: ");
    scanf("%d", &id);
    while (fscanf(fp, "%d,%49[^,],%99[^,],%f,%lf,%19[^\n]\n",
                  &record.plotID, record.ownerName, record.address,
                  &record.landArea, &record.landValue, record.ownershipDate) == 6) 
    {
        if (record.plotID == id) 
        {
            printf("\n✅ Record Found:\n");
            printf("Plot ID: %d\n", record.plotID);
            printf("Owner Name: %s\n", record.ownerName);
            printf("Address: %s\n", record.address);
            printf("Land Area: %.2f sq.ft\n", record.landArea);
            printf("Land Value: %.2lf\n", record.landValue);
            printf("Ownership Date: %s\n", record.ownershipDate);
            found = 1;
            break;
        }
    }
    if (!found)
        printf("\n No record found with Plot ID %d\n", id);
    fclose(fp);
}
void modifyRecord() 
{
    FILE *fp, *temp;
    struct LandRecord record;
    int id, found = 0;
    fp = fopen("land_record.txt", "r");
    temp = fopen("temp.txt", "w");
    if (fp == NULL || temp == NULL) 
    {
        printf("Error opening file!\n");
        return;
    }
    printf("\nEnter Plot ID to modify: ");
    scanf("%d", &id);
    getchar();
    while (fscanf(fp, "%d,%49[^,],%99[^,],%f,%lf,%19[^\n]\n",
                  &record.plotID, record.ownerName, record.address,
                  &record.landArea, &record.landValue, record.ownershipDate) == 6) 
    {
        if (record.plotID == id) 
        {
            found = 1;
            printf("\nEnter new owner name: ");
            fgets(record.ownerName, 50, stdin);
            record.ownerName[strcspn(record.ownerName, "\n")] = '\0';

            printf("Enter new address: ");
            fgets(record.address, 100, stdin);
            record.address[strcspn(record.address, "\n")] = '\0';

            printf("Enter new land value: ");
            scanf("%lf", &record.landValue);
            getchar();

            printf("Enter new ownership date: ");
            fgets(record.ownershipDate, 20, stdin);
            record.ownershipDate[strcspn(record.ownershipDate, "\n")] = '\0';

            printf("\nRecord updated successfully!\n");
        }
        fprintf(temp, "%d,%s,%s,%.2f,%.2lf,%s\n",
                record.plotID, &record.ownerName, record.address,
                record.landArea, record.landValue, record.ownershipDate);
    }
    fclose(fp);
    fclose(temp);
    remove("land_record.txt");
    rename("temp.txt", "land_record.txt");
    if (!found)
    {
        printf("\n No record found with Plot ID %d\n", id);
    }
}
void deleteRecord() 
{
    FILE *fp, *temp;
    struct LandRecord record;
    int id, found = 0;
    fp = fopen("land_record.txt", "r");
    temp = fopen("temp.txt", "w");
    if (fp == NULL || temp == NULL) 
    {
        printf("Error opening file!\n");
        return;
    }
    printf("\nEnter Plot ID to delete: ");
    scanf("%d", &id);
    while (fscanf(fp, "%d,%49[^,],%99[^,],%f,%lf,%19[^\n]\n",
                  &record.plotID, record.ownerName, record.address,
                  &record.landArea, &record.landValue, record.ownershipDate) == 6) 
    {
        if (record.plotID == id) 
        {
            found = 1;
            printf("\nRecord with Plot ID %d deleted successfully!\n", id);
            continue;
        }
        fprintf(temp, "%d,%s,%s,%.2f,%.2lf,%s\n",
                record.plotID, record.ownerName, record.address,
                record.landArea, record.landValue, record.ownershipDate);
    }
    fclose(fp);
    fclose(temp);
    remove("land_record.txt");
    rename("temp.txt", "land_record.txt");
    if (!found)
    {
        printf("\n No record found with Plot ID %d\n", id);
    }
}
