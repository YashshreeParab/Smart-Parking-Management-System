#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX 5

typedef struct {
    char vehicleNumber[20];
    char vehicleType[15];
    int hoursParked;
    float totalFee;
} Vehicle;

/* -------------------------------------------------------------
   DATA STRUCTURE 1: Circular Queue (Waiting Area)
------------------------------------------------------------- */
Vehicle queue[MAX];
int front = -1, rear = -1;

/* -------------------------------------------------------------
   DATA STRUCTURE 2: Stack (Exit History)
------------------------------------------------------------- */
Vehicle stack[MAX];
int top = -1;

/* -------------------------------------------------------------
   NEW DATA STRUCTURE 3: Linked List (Currently Parked Records)
------------------------------------------------------------- */
typedef struct ParkedNode {
    Vehicle vehicle;
    struct ParkedNode* next;
} ParkedNode;

ParkedNode* parkedHead = NULL;
int parkingCount = 0;

/* -------------------------------------------------------------
   DATA STRUCTURE 4: BST for PARKING ZONE STRUCTURE
   Each BST node represents a zone and tracks its occupied slots.
------------------------------------------------------------- */
typedef struct BSTNode {
    char zone[30];
    int capacity;
    int occupied;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

BSTNode *bstRoot = NULL;

/* DATA STRUCTURE 5: Hash Table (fast vehicle-number lookup for parked vehicles) */
#define HASH_SIZE 101
#define HASH_KEY_SIZE 20

typedef struct HashNode {
    char vehicleNumber[HASH_KEY_SIZE];
    char zone[30];
    struct HashNode *next;
} HashNode;

HashNode *hashTable[HASH_SIZE] = {NULL};

unsigned int hashFunction(const char *vehicleNumber) {
    unsigned int hash = 0;
    while (*vehicleNumber) {
        hash = (hash * 31u + (unsigned char)*vehicleNumber) % HASH_SIZE;
        vehicleNumber++;
    }
    return hash;
}

void insertHash(const char *vehicleNumber, const char *zone) {
    unsigned int index = hashFunction(vehicleNumber);
    HashNode *current = hashTable[index];

    /* Update an existing key instead of storing a duplicate. */
    while (current != NULL) {
        if (strcmp(current->vehicleNumber, vehicleNumber) == 0) {
            strcpy(current->zone, zone);
            return;
        }
        current = current->next;
    }

    HashNode *node = (HashNode *)malloc(sizeof(HashNode));
    if (node == NULL) {
        printf("Memory allocation failed; hash index was not updated.\n");
        return;
    }
    strcpy(node->vehicleNumber, vehicleNumber);
    strcpy(node->zone, zone);
    node->next = hashTable[index];
    hashTable[index] = node;
}

HashNode *searchHash(const char *vehicleNumber) {
    unsigned int index = hashFunction(vehicleNumber);
    HashNode *current = hashTable[index];
    while (current != NULL) {
        if (strcmp(current->vehicleNumber, vehicleNumber) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

void deleteHash(const char *vehicleNumber) {
    unsigned int index = hashFunction(vehicleNumber);
    HashNode *current = hashTable[index];
    HashNode *previous = NULL;

    while (current != NULL) {
        if (strcmp(current->vehicleNumber, vehicleNumber) == 0) {
            if (previous == NULL)
                hashTable[index] = current->next;
            else
                previous->next = current->next;
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}


/* -------------------------------------------------------------
   Utility & Validation Functions
------------------------------------------------------------- */
int validVehicle(char num[]) {
    if (strlen(num) != 10) return 0;
    return isalpha(num[0]) && isalpha(num[1]) &&
           isdigit(num[2]) && isdigit(num[3]) &&
           isalpha(num[4]) && isalpha(num[5]) &&
           isdigit(num[6]) && isdigit(num[7]) &&
           isdigit(num[8]) && isdigit(num[9]);
}

void getVehicle(Vehicle *v) {
    do {
        printf("Enter Vehicle Number (e.g., AB12CD3456): ");
        scanf("%19s", v->vehicleNumber);
        for (int i = 0; v->vehicleNumber[i]; i++)
            v->vehicleNumber[i] = toupper(v->vehicleNumber[i]);

        if (!validVehicle(v->vehicleNumber))
            printf("Invalid Vehicle Number format! Must be 2 letters, 2 digits, 2 letters, 4 digits.\n");

    } while (!validVehicle(v->vehicleNumber));

    printf("Enter Vehicle Type (Bike/Car/Truck): ");
    scanf("%14s", v->vehicleType);
    v->hoursParked = 0;
    v->totalFee = 0.0;
}

/* -------------------------------------------------------------
   BST Operations: nodes are PARKING ZONES, not vehicles.
------------------------------------------------------------- */
BSTNode *createBSTNode(const char *zone, int capacity) {
    BSTNode *node = (BSTNode *)malloc(sizeof(BSTNode));
    if (node == NULL) {
        printf("Memory allocation failed while creating parking zone.\n");
        return NULL;
    }
    strcpy(node->zone, zone);
    node->capacity = capacity;
    node->occupied = 0;
    node->left = node->right = NULL;
    return node;
}

BSTNode *insertBST(BSTNode *root, const char *zone, int capacity) {
    if (root == NULL) return createBSTNode(zone, capacity);
    int cmp = strcmp(zone, root->zone);
    if (cmp < 0) root->left = insertBST(root->left, zone, capacity);
    else if (cmp > 0) root->right = insertBST(root->right, zone, capacity);
    return root;
}

BSTNode *searchZone(BSTNode *root, const char *zone) {
    if (root == NULL || strcmp(root->zone, zone) == 0) return root;
    if (strcmp(zone, root->zone) < 0) return searchZone(root->left, zone);
    return searchZone(root->right, zone);
}

void displayZonesInOrder(BSTNode *root) {
    if (root == NULL) return;
    displayZonesInOrder(root->left);
    printf("%s | Occupied: %d | Available: %d\n",
           root->zone, root->occupied, root->capacity - root->occupied);
    displayZonesInOrder(root->right);
}

void displayParkingZones(void) {
    printf("\n--- PARKING ZONES (BST IN-ORDER TRAVERSAL) ---\n");
    displayZonesInOrder(bstRoot);
}

void freeBST(BSTNode *root) {
    if (root == NULL) return;
    freeBST(root->left);
    freeBST(root->right);
    free(root);
}

const char *zoneForVehicleType(const char *vehicleType) {
    char type[15];
    size_t i;
    strncpy(type, vehicleType, sizeof(type) - 1);
    type[sizeof(type) - 1] = '\0';
    for (i = 0; type[i]; i++) type[i] = (char)tolower((unsigned char)type[i]);
    if (strstr(type, "bike") || strstr(type, "two")) return "Zone-A (Two Wheelers)";
    if (strstr(type, "car") || strstr(type, "sedan") || strstr(type, "suv")) return "Zone-B (Light Motor)";
    return "Zone-C (Heavy Vehicles)";
}

/* -------------------------------------------------------------
   Queue Operations
------------------------------------------------------------- */
void enqueue() {
    if ((rear + 1) % MAX == front) {
        printf("Queue Full! Cannot add more waiting vehicles.\n");
        return;
    }
    Vehicle v;
    getVehicle(&v);

    if (front == -1) front = 0;
    rear = (rear + 1) % MAX;
    queue[rear] = v;
    printf("Vehicle %s Added to Waiting Queue.\n", v.vehicleNumber);
}

void dequeue() {
    if (front == -1) {
        printf("Queue Empty! No waiting vehicles to park.\n");
        return;
    }
    if (parkingCount >= MAX) {
        printf("Parking Lot is Full! Cannot allocate parking right now.\n");
        return;
    }

    Vehicle v = queue[front];
    const char *zoneName = zoneForVehicleType(v.vehicleType);
    BSTNode *zoneNode = searchZone(bstRoot, zoneName);
    if (zoneNode == NULL || zoneNode->occupied >= zoneNode->capacity) {
        printf("No available slots in %s. Vehicle remains in the waiting queue.\n", zoneName);
        return;
    }

    ParkedNode *newNode = (ParkedNode *)malloc(sizeof(ParkedNode));
    if (newNode == NULL) {
        printf("Memory allocation failed; vehicle remains in the waiting queue.\n");
        return;
    }
    newNode->vehicle = v;
    newNode->next = NULL;
    if (parkedHead == NULL) parkedHead = newNode;
    else {
        ParkedNode *temp = parkedHead;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newNode;
    }

    /* Remove from circular queue only after allocation succeeds. */
    if (front == rear) front = rear = -1;
    else front = (front + 1) % MAX;
    parkingCount++;
    zoneNode->occupied++;
    insertHash(v.vehicleNumber, zoneName);
    printf("Parking Allocated: %s (%s) assigned to %s\n", v.vehicleNumber, v.vehicleType, zoneName);
}

void displayQueue() {
    if (front == -1) {
        printf("Waiting Queue Empty!\n");
        return;
    }
    int i = front;
    printf("\n--- Waiting Vehicles Queue ---\n");
    while (1) {
        printf("Vehicle: %s | Type: %s\n", queue[i].vehicleNumber, queue[i].vehicleType);
        if (i == rear) break;
        i = (i + 1) % MAX;
    }
}

/* Updated to iterate through Linked List */
void displayParked() {
    if (parkedHead == NULL) {
        printf("Parking Lot is Empty!\n");
        return;
    }
    printf("\n--- Currently Parked Vehicles (Linked List) ---\n");
    ParkedNode* temp = parkedHead;
    int slot = 1;
    while (temp != NULL) {
        printf("Record %d: Vehicle: %s | Type: %s\n", slot++, temp->vehicle.vehicleNumber, temp->vehicle.vehicleType);
        temp = temp->next;
    }
}

/* -------------------------------------------------------------
   Stack Operations
------------------------------------------------------------- */
void push(Vehicle v) {
    if (top == MAX - 1) {
        printf("Exit History Stack Full! Overwriting oldest log not implemented.\n");
        return;
    }
    stack[++top] = v;
    printf("Exit Logged Successfully for %s.\n", v.vehicleNumber);
}

void pop() {
    if (top == -1) {
        printf("Exit History is Empty! No record to remove.\n");
        return;
    }
    printf("Removed Exit Log for: %s\n", stack[top--].vehicleNumber);
}

void peek() {
    if (top == -1) {
        printf("Exit History is Empty! No latest exit to view.\n");
        return;
    }
    printf("\n--- Latest Exit Record ---\n");
    printf("Vehicle: %s (%s) | Duration: %d hrs | Fee: Rs.%.2f\n",
           stack[top].vehicleNumber, stack[top].vehicleType,
           stack[top].hoursParked, stack[top].totalFee);
}

void displayStack() {
    if (top == -1) {
        printf("Exit History Empty!\n");
        return;
    }
    printf("\n--- Complete Exit History Stack ---\n");
    for (int i = top; i >= 0; i--) {
        printf("Vehicle: %s | Type: %s | Duration: %d hrs | Fee Paid: Rs.%.2f\n",
               stack[i].vehicleNumber, stack[i].vehicleType,
               stack[i].hoursParked, stack[i].totalFee);
    }
}

/* -------------------------------------------------------------
   Search & Exit Functions (Stage 2 Integration)
------------------------------------------------------------- */
void searchVehicle() {
    char searchNum[20];
    printf("Enter Vehicle Number to Search: ");
    if (scanf("%19s", searchNum) != 1) {
        printf("Invalid input.\n");
        return;
    }
    for (int i = 0; searchNum[i]; i++)
        searchNum[i] = (char)toupper((unsigned char)searchNum[i]);

    /* Search waiting vehicles in the Circular Queue. */
    if (front != -1) {
        int i = front;
        while (1) {
            if (strcmp(queue[i].vehicleNumber, searchNum) == 0) {
                printf("Status: Found in WAITING QUEUE (Type: %s)\n", queue[i].vehicleType);
                return;
            }
            if (i == rear) break;
            i = (i + 1) % MAX;
        }
    }

    /* Hash table provides fast lookup for currently parked vehicles. */
    HashNode *foundHash = searchHash(searchNum);
    if (foundHash != NULL) {
        printf("Status: Found CURRENTLY PARKED in %s (Hash Table Lookup)\n", foundHash->zone);
        return;
    }

    /* Search completed exit records in the Stack. */
    for (int i = top; i >= 0; i--) {
        if (strcmp(stack[i].vehicleNumber, searchNum) == 0) {
            printf("Status: Found in EXIT HISTORY (Type: %s | Fee Paid: Rs.%.2f)\n",
                   stack[i].vehicleType, stack[i].totalFee);
            return;
        }
    }

    printf("Vehicle %s not found in current records.\n", searchNum);
}

void processExitAndCalculateFee() {
    if (parkedHead == NULL) {
        printf("No vehicles are currently parked. Cannot process exit.\n");
        return;
    }

    char exitNum[20];
    printf("Enter Parked Vehicle Number to Exit: ");
    scanf("%19s", exitNum);
    for (int i = 0; exitNum[i]; i++) exitNum[i] = toupper(exitNum[i]);

    /* Find and remove from Linked List */
    ParkedNode* curr = parkedHead;
    ParkedNode* prev = NULL;
    Vehicle v;
    int found = 0;

    while (curr != NULL) {
        if (strcmp(curr->vehicle.vehicleNumber, exitNum) == 0) {
            v = curr->vehicle;
            found = 1;
            if (prev == NULL) parkedHead = curr->next;
            else prev->next = curr->next;

            free(curr);
            parkingCount--;
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    if (!found) {
        printf("Vehicle %s is not currently parked.\n", exitNum);
        return;
    }

    /* Free the slot recorded in the BST zone node, then remove hash entry. */
    HashNode *exitHashRecord = searchHash(exitNum);
    if (exitHashRecord != NULL) {
        BSTNode *zoneNode = searchZone(bstRoot, exitHashRecord->zone);
        if (zoneNode != NULL && zoneNode->occupied > 0) zoneNode->occupied--;
    }
    deleteHash(exitNum);

    printf("\nProcessing Exit for Vehicle: %s (%s)\n", v.vehicleNumber, v.vehicleType);
    printf("Enter Total Hours Parked: ");
    while (scanf("%d", &v.hoursParked) != 1 || v.hoursParked < 1) {
        printf("Invalid input. Enter positive hours: ");
        while (getchar() != '\n');
    }

    /* Calculate Fee */
    float hourlyRate = 5.0;
    char tempType[15];
    strcpy(tempType, v.vehicleType);
    for (int i = 0; tempType[i]; i++) tempType[i] = tolower(tempType[i]);

    if (strstr(tempType, "bike") || strstr(tempType, "two")) hourlyRate = 20.0;
    else if (strstr(tempType, "car") || strstr(tempType, "sedan") || strstr(tempType, "suv")) hourlyRate = 50.0;
    else if (strstr(tempType, "truck") || strstr(tempType, "bus")) hourlyRate = 100.0;

    v.totalFee = v.hoursParked * hourlyRate;

    printf("\n--- BILL SUMMARY ---\n");
    printf("Vehicle: %s\n", v.vehicleNumber);
    printf("Type: %s\n", v.vehicleType);
    printf("Duration: %d Hours\n", v.hoursParked);
    printf("Hourly Rate: Rs.%.2f\n", hourlyRate);
    printf("Total Amount Due: Rs.%.2f\n", v.totalFee);
    printf("--------------------\n");

    /* Push to Stack */
    push(v);
}

/* -------------------------------------------------------------
   Main Menu
------------------------------------------------------------- */
int main() {
    int choice;

    /* Build the BST of parking zones at startup. Capacity per zone is MAX;
       the overall lot is also limited by MAX parked vehicles. */
    bstRoot = insertBST(bstRoot, "Zone-A (Two Wheelers)", MAX);
    bstRoot = insertBST(bstRoot, "Zone-B (Light Motor)", MAX);
    bstRoot = insertBST(bstRoot, "Zone-C (Heavy Vehicles)", MAX);

    do {
        printf("\n===== SMART PARKING MANAGEMENT =====\n");
        printf("1. Add Vehicle To Queue \n");
        printf("2. Allocate Parking (Move: Queue -> Linked List & BST)\n");
        printf("3. Display Waiting Vehicles\n");
        printf("4. Display Parked Vehicles (Linked List Traversal)\n");
        printf("5. Process Vehicle Exit & Billing\n");
        printf("6. Search Vehicle by Number (Hashing)\n");
        printf("7. Remove Last Exit Record \n");
        printf("8. View Latest Exit Record \n");
        printf("9. Display Complete Exit History\n");
        printf("10. Display Parking Zones (BST)\n");
        printf("11. Exit Program\n");
        printf("Enter Choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid Input! Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1: enqueue(); break;
            case 2: dequeue(); break;
            case 3: displayQueue(); break;
            case 4: displayParked(); break;
            case 5: processExitAndCalculateFee(); break;
            case 6: searchVehicle(); break;
            case 7: pop(); break;
            case 8: peek(); break;
            case 9: displayStack(); break;
            case 10: displayParkingZones(); break;
            case 11: printf("Exiting Smart Parking Management System.\n"); break;
            default: printf("Invalid Choice! Select 1-11.\n");
        }

    } while (choice != 11);

    /* Release dynamically allocated hash-table nodes before termination. */
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode *current = hashTable[i];
        while (current != NULL) {
            HashNode *next = current->next;
            free(current);
            current = next;
        }
        hashTable[i] = NULL;
    }

    freeBST(bstRoot);
    bstRoot = NULL;
    return 0;
}
