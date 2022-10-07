# OS_2022
<p> The project is about creating a program that manages resources. Specifically, the user can book tickets to reserve consecutive seats in the theatre by phone.</p>

## p3200189-p3180146-p3160070-res.c
Here we implemented resource management.
<ul>
  <li> <b>void * processing(void * customer_id)</b> 
    <p>By starting the timer, the customer activates the process of finding an available telephone answering machine. After we associate an answering machine, we randomize the plurality of seats, the response time and the probability of position selection. After the inertia required based on the respond time, we check the zone.By locking the corresponding zone, we start the process of finding positions to be booked.</p>
  <p>Each zone has saved in a Node's list table a sequence of pairs of available seats. If the plurality of the empty seats is greater than or equal to the selected seats then we update the list and start the payment process.</p>
    <p>For payment, the customer waits for the availability of a cashier. When the cashier is associated, we calculate the probability that the customer is available to pay. If we get a positive response, we lock the balance of the theater and add the cost of the tickets to it.</p>
    <p>In the end, we unlock the resources.</p></li>
  <li> <b>int main(int argc, char *argv[])</b>
    <p>First we initialize variables and mutexes, then for each client we call the processing aggregation. At the end we have as output total revenue and times.</p>
  </li>
  <li> <b>void assert_successful_mutex_action(int response_code)</b>
    <p>Function for checking the success of threads.</p>
  </li>

</ul>

## p3200189-p3180146-p3160070-res.h
Creating a Double Link List Node to store the start and end pairs of consecutive available seats.
<p>Example: Let's say we have 10 seats in the theatre. Where 0 means it is available and where 1 means it is not available.</p>
<p>1 | 1 | 0 | 0 | 0 | 1 | 1 | 0 | 0 | 0</p>
<p>Since we have the pairs (2,4) and (7,9), our list will be as follows: 2->4->7->9</p>
<ul>
  <li> <b>void push(struct Node** head_ref, int new_data)</b>
    <p>Given a reference (pointer to pointer) to the head of a list
   and an int, inserts a new node on the front of the list.</p>
  </li>
  <li> <b>void insertAfter(struct Node* prev_node,  int new_data)</b>
    <p>Given a node as prev_node, insert a new node after the given node</p>
  </li>
  <li> <b>void deleteNode(struct Node** head_ref, struct Node* del)</b>
    <p>Deleted a pair of nodes.</p>
  </li>
  <li> <b>void deleteNodebyKey(struct Node** head_ref, int key)</b>
    <p>Deleted a node by the key.</p>
  </li>
  <li> <b>void sortList(struct Node* head_ref)</b>
    <p>Sorted the list, so whenever we add or delete nodes, the list will keep being sorted.</p>
  </li>
  <li> <b>void append(struct Node** head_ref, int new_data)</b>
    <p>Given a reference (pointer to pointer) to the head
   of a DLL and an int, appends a new node at the end</p>
  </li>
  <li> <b>void printList(struct Node* node)</b>
    <p>Printed the list. It has been used for debug.</p>
  </li>
</ul>

## p3180146-p3160070-Copy.c
<p>Here we are testing our Node list.</p>
