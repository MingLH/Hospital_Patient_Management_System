/*
TMF1434 Sem 2 23/24 (G02): Data Structure and Algorithm
Lecture Name: Madam Vanessa Wee Bui Lin
Project Group 2
Student Name: 
1. Lee Hao Ming (99451)
2. Hannah Kho Jing En (99094)
3. Yap Sin Er (101369)
4. Valerie Voon Wen Xin (82987)
*/

#include <iostream>
#include <string>
#include <iomanip>  // for std::setw and std::left

using namespace std;

// Define the Inpatient record
struct InpatientRecord {
	string admissionDate;
	string dischargeDate;
	int lengthOfStay;
	bool admissionStatus;
	bool dischargeStatus;
    InpatientRecord* next;

	//Constructor to initialize InpatientRecord
    InpatientRecord(const string& admDate, const string& disDate, int stay)
        : admissionDate(admDate), dischargeDate(disDate), lengthOfStay(stay), next(NULL) {}
};

// Define the treatment record
struct treatmentRecord {
	string treatmentType;
    string treatmentDate;
    int priority;
    string department;
    InpatientRecord* InpatientHead;
    treatmentRecord* next;

	//Constructor to initialize treatmentRecord
    treatmentRecord(const string& treatment, const string& treatDate, int prio, const string& dptName)
        : treatmentType(treatment), treatmentDate(treatDate), priority(prio), department(dptName), InpatientHead(NULL), next(NULL) {}
};

// Define the patient record
struct patientRecord {
    int patientID;
    string name;
    treatmentRecord* treatmentHead;
    patientRecord* next;

	//Constructor to initialize patientRecord
    patientRecord(int id, const string& pname)
        : patientID(id), name(pname), treatmentHead(NULL), next(NULL) {}
};

// Class to manage patientInventory class with search functionalities
class patientInventory {
private:
    patientRecord* head;
    int nextPatientID;

public:
	//constructor to initialize the patientInventory in public
    patientInventory() : head(NULL), nextPatientID(1) {}
    
    // Helper function to get the length of the linked list
    int getLength() const {
        int length = 0;
        patientRecord* current = head;
        while (current != NULL) {
            length++;
            current = current->next;
        }
        return length;
    }


    // Helper function to generate the next patient ID
    int generatePatientID() {
        return nextPatientID++;
    }
    
    // Helper function to get the nth node of the linked list
    patientRecord* getNthNode(int n) const {
        patientRecord* current = head;
        for (int i = 0; i < n && current != NULL; i++) {
            current = current->next;
        }
        return current;
    }

    // Helper function to perform binary search on the linked list
    patientRecord* binarySearch(int left, int right, int patientID) const {
        while (left <= right) {
        	// calculate the middle index
            int mid = left + (right - left) / 2;
            //get the middle node
            patientRecord* midNode = getNthNode(mid);

			//if the middle node is NULL, return NULL
            if (midNode == NULL) return NULL;

			// if the middle node has desired patientID
            if (midNode->patientID == patientID) {
                return midNode;
            //if the middle node is smaller than the desired patientID
            } else if (midNode->patientID < patientID) {
                left = mid + 1; // search in the right half
            } else {
                right = mid - 1;//searh in the left half
            }
        }
    	//patient NOT found
        return NULL;
    }

    // Add NEW patient record
    bool addPatient( const string& name, const string& treatment, const string& date, int priority, const string& dptName, const string& admDate, const string& disDate,  int lengthOfStay) {
        //generate NEW patientID
		int patientID=generatePatientID();
		//Check if patient with patientID then exist 
		if (searchByID(patientID)) {
            cout << "Patient ID " << patientID << " already exists. Please use a unique ID." << endl;
            return false;
        }
        
        // create NEW patient record if the patientID not founded
   		patientRecord* newPatient = new patientRecord(patientID, name);
   		// add initial treatment Record to the newPatient
   		addTreatment(newPatient, treatment, date, priority, dptName, admDate, disDate, lengthOfStay);
       
       //insert new patient into the linked list , store the patient using linked list
       //if the linked list is empty, it will been added as head 
        if (head == NULL) {
            head = newPatient;
        } else {
        	//find the NULL, and store the new patient as the tail of the linked list
            patientRecord* temp = head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newPatient;
        }
        return true;
    }

    // Add treatment to a patient's record
    void addTreatment(patientRecord* patient, const string& treatment, const string& date, int priority, const string& dptName, const string& admDate, const string& disDate, int lengthOfStay) {
        //Add new treatment to the desired patient
		treatmentRecord* newTreatment = new treatmentRecord(treatment, date, priority, dptName);
        //If it is the first time adding treatment record to patient, link the inpatient record and set the admission and discharge status
        if (patient->treatmentHead == NULL) {
    		InpatientRecord* newInpatient = new InpatientRecord(admDate, disDate, lengthOfStay);
    		newTreatment->InpatientHead = newInpatient;
    		//pre-set the 3 patient to admit to hospital
    		if (patient->name == "Bryan" || patient->name == "Faiz") {
    			newInpatient->admissionStatus = true;
    			patient->treatmentHead = newTreatment;
			}
			else {
				newInpatient->admissionStatus = false;
	    		newInpatient->dischargeStatus = false;
				patient->treatmentHead = newTreatment;
			}
		//If already had existing treatment, it will append the new treatment to the list
        } else {
            treatmentRecord* temp = patient->treatmentHead;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            InpatientRecord* newInpatient = new InpatientRecord(admDate, disDate, lengthOfStay);
    		newTreatment->InpatientHead = newInpatient;
    		newInpatient->admissionStatus = false;
    		newInpatient->dischargeStatus = false;
            temp->next = newTreatment;
       	}	
    }

    // Delete treatment from a patient's record
    bool deleteTreatment(patientRecord* patient, const string& treatment) {
    	//check if the treatment Record is empty or not
        if (patient->treatmentHead == NULL) {
        	return false;
		}
        treatmentRecord* temp = patient->treatmentHead;
        treatmentRecord* prev = NULL;
		
		//Find the treatment to delete
        while (temp != NULL && temp->treatmentType != treatment) {
            prev = temp;
            temp = temp->next;
        }
		
		//If the desired treatment are not found, return false
        if (temp == NULL) {
        	return false; // Treatment not found
		}
		
        if (prev == NULL) { //If the treatment to delete is the head of the list
            patient->treatmentHead = temp->next;
            InpatientRecord* tTemp = temp->InpatientHead;
            delete tTemp;	//to prevent memory leak
    
		} else { //if the node to be deleted is in the middle or end of the list
            prev->next = temp->next;
            InpatientRecord* tTemp = temp->InpatientHead;
            delete tTemp;  //to prevent memory leak
        }
        delete temp;
        return true;
    }
    
    // Add a method to update treatment details for a patient
	bool updateTreatment(patientRecord* patient, const string& oldTreatment, const string& newTreatment, const string& newDate, int newPrio, const string& newDpt, const string& newAdmDate, const string& newDisDate, int newStay) {
	    treatmentRecord* temp = patient->treatmentHead;
	   // Find the treatment to update
	    while (temp != NULL) {
	    	//If the current treatment record matches to the old treatment record
	        if (temp->treatmentType == oldTreatment) {
	        	//If admission date is not set, just update the treatment details
	        	if (temp->InpatientHead->admissionDate == "") {
	        		temp->treatmentType = newTreatment; //update
		            temp->treatmentDate = newDate;//update
		            temp->priority = newPrio;//update
		            temp->department = newDpt;//update
		            
		            return true;// update success
				}
				
				//update treatment details
	            temp->treatmentType = newTreatment;
	            temp->treatmentDate = newDate;
	            temp->priority = newPrio;
	            temp->department = newDpt;
	            
            	InpatientRecord* tTemp = temp->InpatientHead;  // update inpatient
            	tTemp->admissionDate = newAdmDate;
            	tTemp->dischargeDate = newDisDate;
            	tTemp->lengthOfStay = newStay;
            	
            	return true;
	            	
	        }
	        // move to the next treatment record in the linked list
	        temp = temp->next;
	    }
	    return false; // Treatment not found
	}

    // Edit patient information
    bool editPatient(int patientID, const string& newName) {
    	//Search patient by patientID
        patientRecord* patient = searchByID(patientID);
        if (patient == NULL) {
        	//If patient not found, print the message and return false
            cout << "Patient ID " << patientID << " not found." << endl;
            return false;
        }
        //update or edit the patient's new name
        patient->name = newName;
        return true;
    }

	//Display all patients record and their treatment record
    void displayPatients() const {
    	//start from the head of patient linked list 
        patientRecord* temp = head;
        
        cout << "+------------+--------------+-------------------------+----------------+----------+---------------------------+----------------+----------------+----------------+\n";
	    cout << "| Patient ID | Name         | Treatment Type          | Treatment Date | Priority | Department                | Admission Date | Discharge Date | Length of Stay |\n";
	    cout << "+------------+--------------+-------------------------+----------------+----------+---------------------------+----------------+----------------+----------------+\n";
		    
        while (temp != NULL) {
	        
            //Start from the head of treatment linked list for this patient
            treatmentRecord* tTemp= temp->treatmentHead;

            while(tTemp!=NULL){
            	cout << "| " << setw(11) << left << temp->patientID
	                 << "| " << setw(13) << left << temp->name
	                 << "| " << setw(24) << left << tTemp->treatmentType
	                 << "| " << setw(15) << left << tTemp->treatmentDate
	                 << "| " << setw(9) << left << tTemp->priority
	                 << "| " << setw(26) << left << tTemp->department;

            	//If it is an inPatient record associated with treatment, display its details
				if (tTemp->InpatientHead->admissionDate != "") {
            		InpatientRecord* tTemp2= tTemp->InpatientHead;
            		cout << "| " << setw(15) << left << tTemp2->admissionDate
	                     << "| " << setw(15) << left << tTemp2->dischargeDate
	                     << "| " << setw(15) << left << tTemp2->lengthOfStay;
				} else {
	                cout << "| " << setw(15) << left << "-"
	                     << "| " << setw(15) << left << "-"
	                     << "| " << setw(15) << left << "-";
				}
				cout << "|\n";
                tTemp = tTemp->next;
			}
			cout << "+------------+--------------+-------------------------+----------------+----------+---------------------------+----------------+----------------+----------------+\n";
			
			//move to the next patient
            temp = temp->next;
        }
    }
    
 	//display the admission list (
    void displayAdmissionList() const {
    	
    	cout << "+------------+--------------+-------------------------+----------------+----------+--------------+----------------+\n";
	    cout << "| Patient ID | Name         | Treatment Type          | Treatment Date | Priority | Department   | Admission Date |\n";
	    cout << "+------------+--------------+-------------------------+----------------+----------+--------------+----------------+\n";

    	// start from the head
        patientRecord* temp = head;
        while (temp != NULL) {
        	treatmentRecord* tTemp= temp->treatmentHead;
        	while (tTemp != NULL) {
        		static bool firstInPatient = false;
        		//if the first treatment record is In-patient treatment
	        	if (temp->treatmentHead->InpatientHead->admissionDate != "" && firstInPatient == false && temp->treatmentHead->InpatientHead->admissionStatus == false) {
	        		cout << "| " << setw(11) << left << temp->patientID
	                     << "| " << setw(13) << left << temp->name
	                     << "| " << setw(24) << left << tTemp->treatmentType
	                     << "| " << setw(15) << left << tTemp->treatmentDate
	                     << "| " << setw(9) << left << tTemp->priority
	                     << "| " << setw(13) << left << tTemp->department
	                     << "| " << setw(15) << left << tTemp->InpatientHead->admissionDate
	                     << "|\n";
					firstInPatient = true;
					tTemp = tTemp->next;
				}
				else { //if the following treatment record is out patient treatment
					if (tTemp->InpatientHead->admissionDate != "" && tTemp->InpatientHead->admissionStatus == false) {
						cout << "| " << setw(11) << left << temp->patientID
	                         << "| " << setw(13) << left << temp->name
	                         << "| " << setw(24) << left << tTemp->treatmentType
	                         << "| " << setw(15) << left << tTemp->treatmentDate
	                         << "| " << setw(9) << left << tTemp->priority
	                         << "| " << setw(13) << left << tTemp->department
	                         << "| " << setw(15) << left << tTemp->InpatientHead->admissionDate
	                         << "|\n";
						tTemp = tTemp->next;
					}
					else {
						//if the following treatment record is not out patient treatment just move to the next
						tTemp = tTemp->next;
					}
				}
			}
			//move to the next patient
			temp = temp->next;
        }
        cout << "+------------+--------------+-------------------------+----------------+----------+--------------+----------------+\n";
    }

	//Display all admitted patients    
    void displayAdmittedPatients() const {
    	cout << "+------------+--------------+-------------------------+----------------+----------+--------------+----------------+\n";
	    cout << "| Patient ID | Name         | Treatment Type          | Treatment Date | Priority | Department   | Admission Date |\n";
	    cout << "+------------+--------------+-------------------------+----------------+----------+--------------+----------------+\n";
        patientRecord* temp = head;
        while (temp != NULL) {
        	treatmentRecord* tTemp= temp->treatmentHead;
        	while (tTemp != NULL) {
        		static bool firstInPatient = false;
        		//if the first treatment record is In-patient treatment
	        	if (firstInPatient == false && temp->treatmentHead->InpatientHead->admissionStatus == true) {
	        		cout << "| " << setw(11) << left << temp->patientID
	                     << "| " << setw(13) << left << temp->name
	                     << "| " << setw(24) << left << tTemp->treatmentType
	                     << "| " << setw(15) << left << tTemp->treatmentDate
	                     << "| " << setw(9) << left << tTemp->priority
	                     << "| " << setw(13) << left << tTemp->department
	                     << "| " << setw(15) << left << tTemp->InpatientHead->admissionDate
	                     << "|\n";
					firstInPatient = true;
					tTemp = tTemp->next;
				}
				else { //if the following treatment record is out patient treatment
					if (tTemp->InpatientHead->admissionStatus == true) {
						cout << "| " << setw(11) << left << temp->patientID
	                         << "| " << setw(13) << left << temp->name
	                         << "| " << setw(24) << left << tTemp->treatmentType
	                         << "| " << setw(15) << left << tTemp->treatmentDate
	                         << "| " << setw(9) << left << tTemp->priority
	                         << "| " << setw(13) << left << tTemp->department
	                         << "| " << setw(15) << left << tTemp->InpatientHead->admissionDate
	                         << "|\n";
						tTemp = tTemp->next;
					}
					else {
						//if the following treatment record is not out patient treatment just move to the next
						tTemp = tTemp->next;
					}
				}
			}
			//move to the next patient
			temp = temp->next;
        }
        cout << "+------------+--------------+-------------------------+----------------+----------+--------------+----------------+\n";
    }
   
   	//Display all discharged patients    
    void displayDischargedPatients() const {
    	cout << "+------------+--------------+-------------------------+----------------+----------+--------------+----------------+----------------+----------------+\n";
	    cout << "| Patient ID | Name         | Treatment Type          | Treatment Date | Priority | Department   | Admission Date | Discharge Date | Length of Stay |\n";
	    cout << "+------------+--------------+-------------------------+----------------+----------+--------------+----------------+----------------+----------------+\n";
        patientRecord* temp = head;
        while (temp != NULL) {
        	treatmentRecord* tTemp= temp->treatmentHead;
        	while (tTemp != NULL) {
        		static bool firstInPatient = false;
        		//if the first treatment record is In-patient treatment
	        	if (firstInPatient == false && temp->treatmentHead->InpatientHead->dischargeStatus == true) {
	        		cout << "| " << setw(11) << left << temp->patientID
	                     << "| " << setw(13) << left << temp->name
	                     << "| " << setw(24) << left << tTemp->treatmentType
	                     << "| " << setw(15) << left << tTemp->treatmentDate
	                     << "| " << setw(9) << left << tTemp->priority
	                     << "| " << setw(13) << left << tTemp->department
	                     << "| " << setw(15) << left << tTemp->InpatientHead->admissionDate
	                     << "| " << setw(15) << left << tTemp->InpatientHead->dischargeDate
	                     << "| " << setw(15) << left << tTemp->InpatientHead->lengthOfStay
	                     << "|\n";
					firstInPatient = true;
					tTemp = tTemp->next;
				}
				else { //if the following treatment record is out patient treatment
					if (tTemp->InpatientHead->dischargeStatus == true) {
						cout << "| " << setw(11) << left << temp->patientID
	                         << "| " << setw(13) << left << temp->name
	                         << "| " << setw(24) << left << tTemp->treatmentType
	                         << "| " << setw(15) << left << tTemp->treatmentDate
	                         << "| " << setw(9) << left << tTemp->priority
	                         << "| " << setw(13) << left << tTemp->department
	                         << "| " << setw(15) << left << tTemp->InpatientHead->admissionDate
		                     << "| " << setw(15) << left << tTemp->InpatientHead->dischargeDate
		                     << "| " << setw(15) << left << tTemp->InpatientHead->lengthOfStay
	                         << "|\n";
						tTemp = tTemp->next;
					}
					else {
						//if the following treatment record is not out patient treatment just move to the next
						tTemp = tTemp->next;
					}
				}
			}
			//move to the next patient
			temp = temp->next;
        }
        cout << "+------------+--------------+-------------------------+----------------+----------+--------------+----------------+----------------+----------------+\n";
    }

	//Display all outpatients
    void displayOutPatients() const {
    	
    	cout << "+------------+--------------+-------------------------+----------------+----------+---------------------------+\n";
	    cout << "| Patient ID | Name         | Treatment Type          | Treatment Date | Priority | Department                |\n";
	    cout << "+------------+--------------+-------------------------+----------------+----------+---------------------------+\n";

        patientRecord* temp = head;
        while (temp != NULL) {
        	treatmentRecord* tTemp= temp->treatmentHead;
        	while (tTemp != NULL) {
        		static bool firstOutpatient = false;
        		//if the first treatment record is out patient treatment
	        	if (temp->treatmentHead->InpatientHead->admissionDate == "" && firstOutpatient == false) {
	        		cout << "| " << setw(11) << left << temp->patientID
	                     << "| " << setw(13) << left << temp->name
	                     << "| " << setw(24) << left << tTemp->treatmentType
	                     << "| " << setw(15) << left << tTemp->treatmentDate
	                     << "| " << setw(9) << left << tTemp->priority
	                     << "| " << setw(26) << left << tTemp->department
	                     << "|\n";

					firstOutpatient = true;
					tTemp = tTemp->next;
				}
				else { //if the following treatment record is out patient treatment
					if (tTemp->InpatientHead->admissionDate == "") {
						cout << "| " << setw(11) << left << temp->patientID
		                     << "| " << setw(13) << left << temp->name
		                     << "| " << setw(24) << left << tTemp->treatmentType
		                     << "| " << setw(15) << left << tTemp->treatmentDate
		                     << "| " << setw(9) << left << tTemp->priority
		                     << "| " << setw(26) << left << tTemp->department
		                     << "|\n";
						tTemp = tTemp->next;
					}
					else {
						//if the following treatment record is not out patient treatment just move to the next
						tTemp = tTemp->next;
					}
				}
			}
			//move to the next patient
			temp = temp->next;
        }
        cout << "+------------+--------------+-------------------------+----------------+----------+---------------------------+\n";
    }

    // Search for a patient by ID using binary search
    patientRecord* searchByID(int patientID) const {
		// Get the length of the linked list
        int length = getLength();
		//perform binary search
        return binarySearch(0, length - 1, patientID);
    }

    // Search for patients by name
    void searchByName(const string& name) const {
	    
        patientRecord* temp = head;
        bool found = false; //initialize found to false at the start of the search
        while (temp != NULL) {
            //check if the current patient name matches to the search name
			if (temp->name == name) {
				
				cout << "+------------+--------------+-------------------------+----------------+----------+---------------------------+----------------+----------------+----------------+\n";
			    cout << "| Patient ID | Name         | Treatment Type          | Treatment Date | Priority | Department                | Admission Date | Discharge Date | Length of Stay |\n";
			    cout << "+------------+--------------+-------------------------+----------------+----------+---------------------------+----------------+----------------+----------------+\n";
			    
                treatmentRecord* tTemp = temp->treatmentHead;
                while (tTemp != NULL) {
                    cout << "| " << setw(11) << left << temp->patientID
	                     << "| " << setw(13) << left << temp->name
	                     << "| " << setw(24) << left << tTemp->treatmentType
	                     << "| " << setw(15) << left << tTemp->treatmentDate
	                     << "| " << setw(9) << left << tTemp->priority
	                     << "| " << setw(26) << left << tTemp->department;
	                     
                    if (tTemp->InpatientHead->admissionDate != "") {
                    	InpatientRecord* tTemp2 = tTemp->InpatientHead;
                    	cout << "| " << setw(15) << left << tTemp2->admissionDate
                         	 << "| " << setw(15) << left << tTemp2->dischargeDate
                         	 << "| " << setw(15) << left << tTemp2->lengthOfStay;
					} else {
	                    cout << "| " << setw(15) << left << "-"
	                         << "| " << setw(15) << left << "-"
	                         << "| " << setw(15) << left << "-";
	                }
	                cout << "|\n";
					tTemp = tTemp->next;
                }
                cout << "+------------+--------------+-------------------------+----------------+----------+---------------------------+----------------+----------------+----------------+\n";
                // set found to true since a match has been found
                found = true;
            }
            //if not been found the node move to the next node
            temp = temp->next;
        }
        if (!found) {
            cout << "No patients found with the name " << name << "." << endl;
        }
    }

    // Search for patients by treatment type
    void searchByTreatment(const string& treatment) const {
    	
    	cout << "+------------+--------------+-------------------------+----------------+----------+---------------------------+----------------+----------------+----------------+\n";
	    cout << "| Patient ID | Name         | Treatment Type          | Treatment Date | Priority | Department                | Admission Date | Discharge Date | Length of Stay |\n";
	    cout << "+------------+--------------+-------------------------+----------------+----------+---------------------------+----------------+----------------+----------------+\n";
    	
        patientRecord* temp = head;
        bool found = false;
        while (temp != NULL) {
            treatmentRecord* tTemp = temp->treatmentHead;
            while (tTemp != NULL) {
                if (tTemp->treatmentType == treatment) {
                	cout << "| " << setw(11) << left << temp->patientID
	                     << "| " << setw(13) << left << temp->name
	                     << "| " << setw(24) << left << tTemp->treatmentType
	                     << "| " << setw(15) << left << tTemp->treatmentDate
	                     << "| " << setw(9) << left << tTemp->priority
	                     << "| " << setw(26) << left << tTemp->department;
	                     
            		if (tTemp->InpatientHead->admissionDate != "") {
	                    InpatientRecord* tTemp2 = tTemp->InpatientHead;
	                    cout << "| " << setw(15) << left << tTemp2->admissionDate
	                         << "| " << setw(15) << left << tTemp2->dischargeDate
	                         << "| " << setw(15) << left << tTemp2->lengthOfStay;
	                } else {
	                    cout << "| " << setw(15) << left << "-"
	                         << "| " << setw(15) << left << "-"
	                         << "| " << setw(15) << left << "-";
	                }
	                cout << "|\n";
					//set found to true since a match has been found
					found = true;
                }
                //move to next treatment
				tTemp = tTemp->next;
            }
            //move to next patient
            temp = temp->next;
        }
        cout << "+------------+--------------+-------------------------+----------------+----------+---------------------------+----------------+----------------+----------------+\n";
        
        //if no matches are found
        if (!found) {
            cout << "No patients found with the treatment type " << treatment << "." << endl;
        }
    }

    // Sort patients by treatment date
    void sortByTreatmentDate() {
    	// No need to sort if 0 or 1 element
        if (head == NULL || head->next == NULL) {
			return;
		} 
        patientRecord* current = head; // pointer to traverse list
        patientRecord* nextNode = NULL; // pointer to mark the end of the sorted portion of the list
        bool sorted = false;// assume the list need sorting

		//bubble sort algorithm
        while (!sorted) {
            sorted = true; //assume list is sorted at beginning of each pass
            current = head;
            while (current->next != nextNode) {
                // Change the comparison to sort from most recent to least recent
                if (current->treatmentHead->treatmentDate < current->next->treatmentHead->treatmentDate) {
                    //swap
					swapPatients(current, current->next);
                    //found a pair that need to be swapped
					sorted = false;
                }
                //move to the next pair
                current = current->next;
            }
            //the last element of this pass in correct position
            nextNode = current;
        }
    }

    // Sort patients by priority
    void sortByPriority() {
    	// No need to sort if 0 or 1 element
        if (head == NULL || head->next == NULL) {
			return;
		} 
        patientRecord* current = head;
        patientRecord* nextNode = NULL;
        bool sorted = false;

        while (!sorted) {
            sorted = true;//assume list is sorted at beginning of each pass
            current = head;
            while (current->next != nextNode) {
                // Change the comparison to sort from most recent to least recent                
				if (current->treatmentHead->priority > current->next->treatmentHead->priority) {
                    swapPatients(current, current->next);
                    //found a pair that need to be swapped                    
					sorted = false;
                }
                //move to the next pair                
				current = current->next;
            }
            //the last element of this pass in correct position
            nextNode = current;
        }
    }

    void sortByLengthOfStay() {
	    if (head == NULL || head->next == NULL) {
	        return; // No need to sort if 0 or 1 element
	    }
	
	    patientRecord* current = head;
	    patientRecord* nextNode = NULL;
	    bool sorted = false;
	
	    while (!sorted) {
	        sorted = true;//assume list is sorted at beginning of each pass
	        current = head;
	        while (current->next != nextNode) {
                // Change the comparison to sort from most recent to least recent              
			    if (current->treatmentHead->InpatientHead->lengthOfStay < current->next->treatmentHead->InpatientHead->lengthOfStay) {
                    swapPatients(current, current->next);
                     //found a pair that need to be swapped                   
					sorted = false;
                }
                //move to the next pair	            
				current = current->next;
	        }
            //the last element of this pass in correct position
	        nextNode = current;
	    }
	}
	
	// Sort patients by department
    void sortByDepartment() {
    	// No need to sort if 0 or 1 element
        if (head == NULL || head->next == NULL) {
			return;
		}
        patientRecord* current = head;
        patientRecord* nextNode = NULL;
        bool sorted = false;

        while (!sorted) {
            sorted = true;//assume list is sorted at beginning of each pass
            current = head;
            while (current->next != nextNode) {
                // Change the comparison to sort from most recent to least recent                
				if (current->treatmentHead->department > current->next->treatmentHead->department) {
                    swapPatients(current, current->next);
                    //found a pair that need to be swapped                    
					sorted = false;
                }
                //move to the next pair               
			    current = current->next;
            }
            //the last element of this pass in correct position
	           nextNode = current;
        }
    }

    // Helper function to swap two patient records
    void swapPatients(patientRecord* a, patientRecord* b) {
        // Swap patient ID, name
        swap(a->patientID, b->patientID);
        swap(a->name, b->name);

        // Swap treatment head (linked list remains intact)
        treatmentRecord* tempTreatmentHead = a->treatmentHead;
        a->treatmentHead = b->treatmentHead;
        b->treatmentHead = tempTreatmentHead;
    }

//Function to perform insertion sort on the linked list of patient record
	void linkedInsertionSort() {
		patientRecord *lastInOrder; //pointer to the last element of sorted portion
		patientRecord *firstOutOfOrder; // pointer to the first element of unsorted portion
		patientRecord *current; //pointer to traverse the list 
		patientRecord *trailCurrent; //pointer to keep track of previous element
		
		lastInOrder = head;
		
		//check if is empty
		if (head == NULL) {
			cerr << "Cannot sort an empty list." << endl;
		}
		//check if only one element
		else if (head->next == NULL) {
			cout << "The list is of length 1. "
			<< "It is already in order." << endl;
		}
		//check if more than one element
		else {
			//loop through unsorted portion of list
			while (lastInOrder->next != NULL) {
				firstOutOfOrder = lastInOrder->next;
				
				//if the first out of order element is less than the head, insert it at beginning
				if (firstOutOfOrder->patientID < head->patientID) {
					lastInOrder->next = firstOutOfOrder->next;
					firstOutOfOrder->next = head;
					head = firstOutOfOrder;
				}
				//traverse the sorted portion to find the correct position
				else {
					trailCurrent = head;
					current = head->next;
					while (current->patientID < firstOutOfOrder->patientID) {
						trailCurrent = current;
						current = current->next;
					}
					//insert the first out-of-order element  in its correct position
					if (current != firstOutOfOrder) {
						lastInOrder->next = firstOutOfOrder->next;
						firstOutOfOrder->next = current;
						trailCurrent->next = firstOutOfOrder;
					}
					else {
						//if element in correct position, move the lastInOrder pointer
						lastInOrder = lastInOrder->next;
					}
				}
			} //end while
		}
	} //end linkedInsertionSort

    // Destructor to free allocated memory
    ~patientInventory() {
        while (head != NULL) {
        	//loop through all patient records and free the memory
            patientRecord* temp = head;
            head = head->next;
            //free memory allocated for treatment record
            while (temp->treatmentHead != NULL) {
                treatmentRecord* tTemp = temp->treatmentHead;
                temp->treatmentHead = tTemp->next;
                delete tTemp;
            }
            delete temp;
        }
    }
    
    // Getter for the next outpatient ID
    int getNextPatientID() const {
        return nextPatientID;
    }
	
};


// Define the Node structure for the queue
struct Node {
    patientRecord* data;//pointer to patient record
    Node* next;// pointer to next node in queue

	//constructor to initialize node with patient record
    Node(patientRecord* record) : data(record), next(NULL) {}
};


// Define the AdmissionQueue class
class AdmissionQueue {
public:
    Node* queueFront;
    Node* queueRear;

//constructor to initialize AdmissionQueue
    AdmissionQueue() : queueFront(NULL), queueRear(NULL) {}

    // Method to add a patient to the queue
    void enqueue(patientRecord* record) {
        Node* newNode = new Node(record);
        //if the queue is empty
        if (queueRear == NULL) {
            queueFront = queueRear = newNode;
            return;
        }
        //if queue not empty
    	queueRear->next = newNode;
    	queueRear = newNode;
    	record->treatmentHead->InpatientHead->dischargeStatus = false;
    }

    // Method to remove a patient from the queue
    patientRecord* dequeue() {
    	//if queue is empty
        if (queueFront == NULL) {
            cout << "Admission Queue is empty" << endl;
            return NULL;
        }
        //store the current node in temporary variable
        Node* temp = queueFront;
        //move the front pointer to next node
        queueFront = queueFront->next;
        //if front is NULL mean rear pointer to NULL also
        if (queueFront == NULL) {
            queueRear = NULL;
        }
        
        //retrieve patient record 
        patientRecord* record = temp->data;
        //patient admitted by setting admissionstatus is true
        record->treatmentHead->InpatientHead->admissionStatus = true;
        cout << "Admit Patient ID " << record->patientID << " - " << record->name << endl;
        delete temp;
        return record;
    }

    // Method to check if the queue is empty
    bool isEmpty() {
        return queueFront == NULL;
    }

    // Method to display the queue
    void displayQueue() {
    	
    	cout << "+------------+--------------+----------+--------------+----------------+\n";
        cout << "| Patient ID | Name         | Priority | Department   | Admission Date |\n";
        cout << "+------------+--------------+----------+--------------+----------------+\n";
    	
        Node* temp = queueFront;
        if (temp == NULL) {
            cout << "Admission Queue is empty" << endl;
            return;
        }
        while (temp != NULL) {
            cout << "| " << setw(11) << left << temp->data->patientID
                 << "| " << setw(13) << left << temp->data->name
                 << "| " << setw(9) << left << temp->data->treatmentHead->priority
                 << "| " << setw(13) << left << temp->data->treatmentHead->department
                 << "| " << setw(15) << left << temp->data->treatmentHead->InpatientHead->admissionDate
                 << "|\n";
            temp = temp->next;
        }
        cout << "+------------+--------------+----------+--------------+----------------+\n";
    }

    // Destructor to clean up memory
    ~AdmissionQueue() {
        while (queueFront != NULL) {
            dequeue();
        }
    }
};

// Define the Discharge class
class DischargeQueue {
public:
    Node* queueFront;
    Node* queueRear;

//constructor to initializes dischargequeue
    DischargeQueue() : queueFront(NULL), queueRear(NULL) {}

    // Method to add a patient to the queue
    void enqueue(patientRecord* record) {
    	//create new node
        Node* newNode = new Node(record);
        //check if queue is empty
		if (queueRear == NULL) {
            queueFront = queueRear = newNode;
            return;
        }
		//add new node at the end of the queue
		queueRear->next = newNode;
        //update queueRear= new node
		queueRear = newNode;
    }

    // Method to remove a patient from the queue
    patientRecord* dequeue() {
        if (queueFront == NULL) {
            cout << "Dischared Queue is empty" << endl;
            return NULL;
        }
        //store the current front node to temporary
        Node* temp = queueFront;
        //move queueFront pointer to next node
        queueFront = queueFront->next;
        //If queueFront is empty queueRear is empty
		if (queueFront == NULL) {
            queueRear = NULL;
        }
        //retrieve patient record
        patientRecord* record = temp->data;
        //mark the patient as discharged 
        record->treatmentHead->InpatientHead->dischargeStatus = true;
        record->treatmentHead->InpatientHead->admissionStatus = false;
        cout << "Discharge Patient ID " << record->patientID << " - " << record->name << endl;
        //delete temporart node to free memory
		delete temp;
		//return patient record 
        return record;
    }

    // Method to check if the queue is empty
    bool isEmpty() {
        return queueFront == NULL;
    }

    // Method to display the queue
    void displayQueue() {
        
    	cout << "+------------+--------------+----------+--------------+----------------+----------------+----------------+\n";
        cout << "| Patient ID | Name         | Priority | Department   | Admission Date | Discharge Date | Length of Stay |\n";
        cout << "+------------+--------------+----------+--------------+----------------+----------------+----------------+\n";

        Node* temp = queueFront;
        
        if (temp == NULL) {
            cout << "Discharge Queue is empty" << endl;
            return;
        }
        
        while (temp != NULL) {
            cout << "| " << setw(11) << left << temp->data->patientID
                 << "| " << setw(13) << left << temp->data->name
                 << "| " << setw(9) << left << temp->data->treatmentHead->priority
                 << "| " << setw(13) << left << temp->data->treatmentHead->department
                 << "| " << setw(15) << left << temp->data->treatmentHead->InpatientHead->admissionDate
                 << "| " << setw(15) << left << temp->data->treatmentHead->InpatientHead->dischargeDate
                 << "| " << setw(15) << left << temp->data->treatmentHead->InpatientHead->lengthOfStay
                 << "|\n";
            temp = temp->next;
        }
        cout << "+------------+--------------+----------+--------------+----------------+----------------+----------------+\n";
    }

    // Destructor to clean up memory
    ~DischargeQueue() {
        while (queueFront != NULL) {
            dequeue();
        }
    }
};


void displayMenu() {
    cout << "\n==== Hospital Management System ====" << endl;
    cout << "1. Patient Data Management" << endl;
    cout << "2. Display Patient Data" << endl;
    cout << "3. Admission and Discharge" << endl;
    cout << "4. Search Patients" << endl; // New option for searching patients
    cout << "5. Sort Patients" << endl; // New option for sorting patients
    cout << "6. Generate Summary Report" << endl;
    cout << "7. Exit" << endl;
    cout << "====================================" << endl;
    cout << "Choose an option (1-7): ";
	
}

void displayInventoryMenu() {
    cout << "\n==== Patient Data Management ====" << endl;
    cout << "1. Add Patient" << endl;
    cout << "2. Edit Patient Name" << endl;
    cout << "3. Add Treatment to Patient" << endl;
    cout << "4. Delete Treatment from Patient" << endl;
    cout << "5. Edit Treatment from Patient" << endl;
    cout << "=================================" << endl;
    cout << "Choose an option (1-5): ";
}

void displayAdmDisMenu() {
    cout << "\n==== Admission and Discharge ====\n";
    cout << "Admission:" << endl;
    cout << "1. Schedule for Patient Admission" << endl;
    cout << "2. Display Admission Queue" << endl;
    cout << "3. Admit Patient" << endl;
    cout << "4. Display Admitted Patient" << endl << endl;
    cout << "Discharge:" << endl;
    cout << "5. Schedule for Patient Discharge" << endl;
    cout << "6. Display Discharged Queue" << endl;
    cout << "7. Discharge Patient" << endl;
    cout << "8. Display Discharged Patient" << endl;
    cout << "=================================\n";
    cout << "Choose an option (1-8): ";
}

void displaySearchMenu() {
    cout << "\n===== Search Patients =====\n";
    cout << "1. Search by Name" << endl;
    cout << "2. Search by Patient ID" << endl;
    cout << "3. Search by Treatment Type" << endl;
    cout << "===========================\n";
    cout << "Choose an option (1-3): ";
}

void displaySortMenu() {
    cout << "\n======= Sort Patients ========\n";
    cout << "1. Sort by Treatment Date" << endl;
    cout << "2. Sort by Priority" << endl;
    cout << "3. Sort by Length of Stay" << endl;
    cout << "4. Sort by Patient ID (default)" << endl;
    cout << "===============================\n";
    cout << "Choose an option (1-4): ";
}

int main() {
    // Create an inventory and a queue
    patientInventory inventory;
    AdmissionQueue admissionQueue;
    DischargeQueue dischargeQueue;

    // Add predefined patients to the inventory and queue
	// int prePatientIDs[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
	// predefined patients data
    string prePatientNames[] = {"Marc", "Hafiz", "Bryan", "Mei Ling", "Shativel", "Faiz", "Li Wei", "Sathesh", "Jason", "Daniel"};
    string prePatientTreatments[] = {"Electrocardiograms", "Chemotherapy", "Radiation Therapy", "Pelvic Exams", "X-rays", "Bone Marrow Transplant", "CT Scans", "Endoscopy", "Chemotherapy", "Psychotherapy"};
    string preTreatmentDates[] = {"2024-01-15", "2024-04-28", "2024-03-12", "2024-03-05", "2024-05-18", "2024-02-14", "2023-10-22", "2023-08-10", "2023-12-14", "2024-01-29"};
    int prePriorities[] = {2, 1, 3, 1, 2, 3, 1, 2, 3, 1};
    string preDepartment[] = {"Cardiology", "Oncology", "Oncology", "Obstetrics and Gynecology", "Radiology", "Hematology", "Radiology", "Gastroenterology", "Oncology", "Psychology"};
    string preAdmissionDate[] = {"", "2024-04-20", "2024-03-09", "", "", "2024-02-02", "", "", "2023-12-07", ""};
    string preDischargeDate[] = {"", "2024-05-08", "2024-03-16", "", "", "2024-03-15", "", "", "2023-12-23", ""};
    int preLengthOfStay[] = {0, 19, 7, 0, 0, 42, 0, 0, 16, 0};

	// Add predefined patients to the inventory
    for (int i = 0; i < 10; i++) {
        if (inventory.addPatient(prePatientNames[i], prePatientTreatments[i], preTreatmentDates[i], prePriorities[i], preDepartment[i], preAdmissionDate[i], preDischargeDate[i], preLengthOfStay[i])) {
			continue;
		}
	}
	
	// Add pre-set schedule for patient discharge
	patientRecord* preAdmit1 = inventory.searchByID(3);
	dischargeQueue.enqueue(preAdmit1);
	
	patientRecord* preAdmit2 = inventory.searchByID(6);
	dischargeQueue.enqueue(preAdmit2);

    int choice;
    do {
        displayMenu();  // Display the main menu and get user choice
        cin >> choice;
        
        switch (choice) {
            case 1: {
            	system("pause");
				system("CLS");
                int option;
                // Display the inventory menu and get user option
                displayInventoryMenu();  
                cin >> option;

                switch (option) {
                    case 1: {
				        system("pause");
				        system("CLS");
				        
                        string name, treatment, treatmentDate, department;
                        int priority;
                        bool isInpatient;
                        
                        // Collect patient information from the user
                        cout << "\n=================== Patient Information ===================" << endl;
                        cout << "Enter Patient Name: ";
                        cin.ignore();     // Ignore leftover newline character
                        getline(cin, name);
                        cout << "Enter Treatment Type: ";
                        getline(cin, treatment);     // Get treatment type
                        cout << "Enter Treatment Date (YYYY-MM-DD): ";
              			getline(cin, treatmentDate);   // Get treatment Date
               			cout << "Enter Priority (1-highest 2-medium 3-lowest): ";  
              			cin >> priority;              // Get priority level
              			cout << "Enter Department: ";
              			cin.ignore();        // Ignore leftover newline character
                        getline(cin, department);     // Get department
                        cout << "Is the patient required admission (1 for YES, 0 for NO): " ;
                        cin>> isInpatient;            // Get admission status
                        cout << endl;
                        
                        // If successful, admit to the queue if inpatient
                        if (isInpatient) {
                        	string admissionDate, dischargeDate;
                        	int lengthOfStay;
                        	
                        	// Collect inpatient information
                        	cout << "==== Information for Patient Required Admission ====" << endl;
                        	cout << "Enter Admission Date (YYYY-MM-DD): ";
                        	cin.ignore();              // Ignore leftover newline character
              				getline(cin, admissionDate);      // Get admission date
              				cout << "Enter Discharge Date (YYYY-MM-DD): ";
              				getline(cin, dischargeDate);      // Get discharge date
                    		cout << "Enter Length of Stay (in days): ";
                    		cin >> lengthOfStay;              // Get length of stay
                    		
                    		 // Add patient with inpatient details to inventory
                    		if (inventory.addPatient(name, treatment, treatmentDate, priority, department, admissionDate, dischargeDate, lengthOfStay)){
	                            cout << endl << "----- Patient Information Added -----" << endl << endl;
	                            system("pause");
				        		system("CLS");
							}
						}
						else {
							// Add patient without inpatient details to inventory
							if (inventory.addPatient(name, treatment, treatmentDate, priority, department, "" , "" , 0)){
	                            cout << "----- Patient Information Added -----" << endl << endl;
	                            system("pause");
				        		system("CLS");
							}
						}
                        break;
                    }
                    case 2: {
                    	system("pause");
				        system("CLS");
				        
				        cout << "==== Patient Record ====" << endl;
				        inventory.displayPatients();
				        
                    	// Edit patient name
                    	cout << "\n====== Edit Patient Name ======" << endl;
                        int patientID; 
						string newName;
		                cout << "Enter Patient ID: ";
		                cin >> patientID;
		            	
		            	//check the patient record to find the patient ID
		                patientRecord* patient = inventory.searchByID(patientID);
		                if (patient != NULL) {
		                    cout << "Enter New Patient Name: ";
		                    cin.ignore();               // Ignore leftover newline character
			                getline(cin, newName);     // Get new patient name
			                
			                // Edit patient information
			                if (inventory.editPatient(patientID, newName)) {
			            	cout << endl << "-- Patient Information Updated --" << endl << endl;
			            	system("pause");
				        	system("CLS");
			                }
		                } else {
		                    cout << endl << "-- Patient ID not found --" << endl << endl;
		                    system("pause");
				        	system("CLS");
		                }
						break;
					}
                    case 3: {
                    	system("pause");
				        system("CLS");
				        
				        cout << "==== Patient Record ====" << endl;
				        inventory.displayPatients();
				        
                    	int patientID;
		                cout << "\n==== Add Treatment to Patient =====" << endl;
		                cout << "Enter Patient ID: ";
		                cin >> patientID;
		                
		                patientRecord* patient = inventory.searchByID(patientID);
		                if (patient != NULL) {
		                	string treatment, treatmentDate, department;
		                	int priority;
		                	bool isInpatient;
		                	
		                	// Collect treatment information
			                cout << "Enter Treatment Type: ";
			                cin.ignore();                 // Ignore leftover newline character
		                    getline(cin, treatment);      // Get treatment Type
		                    cout << "Enter Treatment Date (YYYY-MM-DD): ";
		                    getline(cin, treatmentDate);
		                    cout << "Enter Priority (1-highest 2-medium 3-lowest): ";
		          			cin >> priority;              // Get priority level
		          			cout << "Enter Deapartment: ";
		          			cin.ignore();
		                    getline(cin, department);      // Get department
		                    cout << "Is the patient being admitted (1 for YES, 0 for NO): " ;
		                    cin>> isInpatient;            // Get admission status
		                    
		                    if (isInpatient) {
		                    	string admissionDate, dischargeDate;
		                    	int lengthOfStay;
		                    	
		                    	cout << "Enter Admission Date (YYYY-MM-DD): ";
		                    	cin.ignore();         
		          				getline(cin, admissionDate);          // Get admission date
		          				cout << "Enter Discharge Date (YYYY-MM-DD): ";
		          				getline(cin, dischargeDate);          // Get discharge date
		                		cout << "Enter Length of Stay (in days): ";
		                		cin >> lengthOfStay;                  // Get length of stay
		                		
		                		 // Add treatment with inpatient details to inventory
		                		inventory.addTreatment(patient, treatment, treatmentDate, priority, department, admissionDate, dischargeDate, lengthOfStay);
		                        cout << endl << "----- Treatment Added -----" << endl;
		                        
		                        system("pause");
				        		system("CLS");
							}
							else {
								 // Add treatment without inpatient details to inventory
								inventory.addTreatment(patient, treatment, treatmentDate, priority, department, "" , "" , 0);
		                        cout << endl << "----- Treatment Added -----" << endl;
		                        
		                        system("pause");
				        		system("CLS");
								}
							} else {
		                    cout << endl << "----- Patient ID not found -----" << endl;
		                    system("pause");
				        	system("CLS");
		                }
						break;
					}
                    case 4: {
                    	system("pause");
				        system("CLS");
				        
                        int patientID;
						string treatment;
						
						cout << "==== Patient Record ====" << endl;
						inventory.displayPatients();
						
						// Delete treatment from patient
		                cout << "\n==== Delete Treatment from Patient =====" << endl;
		                
		                cout << "Enter Patient ID: ";
		                cin >> patientID;
		
		                // Check the patient record to find the patient ID
		                patientRecord* patient = inventory.searchByID(patientID);
		                if (patient != NULL) {
			                cout << "Enter Treatment Type to Delete: ";
			                cin.ignore();
			                getline(cin, treatment);          // Get treatment type
			                
			                // Delete treatment from patient
		                    if (inventory.deleteTreatment(patient, treatment)) {
		                        cout << "\n----- Treatment deleted -----\n" << endl;
		                        system("pause");
				        		system("CLS");
		                    } else {
		                        cout << "\n----- Treatment not found for patient -----\n" << endl;
		                        system("pause");
				        		system("CLS");
		                    }
		                } else {
		                    cout << "\n----- Patient ID not found -----\n" << endl;
		                    system("pause");
				        	system("CLS");
		                }
		                break;
                    }
                    case 5: {
                    	system("pause");
				        system("CLS");
                    	
                        int patientID;
                        
                        cout << "==== Patient Record ====" << endl;
                        inventory.displayPatients();
                        
                        // Edit treatment from patient
					    cout << "\n==== Edit Treatment from Patient =====" << endl;
					    cout << "Enter Patient ID: ";
					    cin >> patientID;
				        
					    // Check the patient record to find the patient ID
		                patientRecord* patient = inventory.searchByID(patientID);
		                if (patient != NULL) {
		                	
		                	// Check if the patient is an inpatient
		                	if (patient->treatmentHead->InpatientHead->admissionDate == "") {
		                		string oldTreatment, newTreatment, treatmentDate, department;
				                int priority;
			                	
			                	// Collect new treatment information
			                	cout << "Enter Treatment Type to Edit: ";
			                	cin.ignore();                               // Ignore leftover newline character
							    getline(cin, oldTreatment);                 // Get old treatment type
							    cout << "Enter New Treatment Type: ";
							    getline(cin, newTreatment);                 // Get new treatment type
							    cout << "Enter New Treatment Date (YYYY-MM-DD): ";
							    getline(cin, treatmentDate);                // Get new treatment date
			                    cout << "Enter New Priority (1-highest 2-medium 3-lowest): ";
			          			cin >> priority;                            // Get new priority level
			          			cout << "Enter New Deapartment: ";
			          			cin.ignore();
			                    getline(cin, department);                   // Get new department
			                    
			                    // Update treatment without inpatient details
			                    if (inventory.updateTreatment(patient, oldTreatment, newTreatment, treatmentDate, priority, department, "-", "-", 0)) {
			                    	cout << "\n----- Treatment Edited -----\n" << endl;
			                    	system("pause");
				        			system("CLS");
								}
								else {
									cout << "\n----- Treatment Not Found for patient ID -----\n" << patient->patientID << endl;
									system("pause");
				        			system("CLS");
								}
							}
							else {
								string oldTreatment, newTreatment, treatmentDate, department, admissionDate, dischargeDate;
				                int priority, lengthOfStay;
			                	
			                	// Collect new treatment information
			                	cout << "Enter Treatment Type to Edit: ";
			                	cin.ignore();
							    getline(cin, oldTreatment);              // Get old treatment type
							    
							    cout << "Enter New Treatment Type: ";
							    getline(cin, newTreatment);              // Get new treatment type
							    
							    cout << "Enter New Treatment Date (YYYY-MM-DD): ";
							    getline(cin, treatmentDate);             // Get new treatment date
							    
			                    cout << "Enter New Priority (1-highest 2-medium 3-lowest): ";
			          			cin >> priority;                         // Get new priority level
			          			
			          			cout << "Enter New Deapartment: ";
			          			cin.ignore();
			                    getline(cin, department);                // Get new department
			                    
			                	cout << "Enter Admission Date (YYYY-MM-DD): ";
			      				getline(cin, admissionDate);                 // Get new admission date
			      				
			      				cout << "Enter Discharge Date (YYYY-MM-DD): ";
			      				getline(cin, dischargeDate);                   // Get new discharge date
			      				
			            		cout << "Enter Length of Stay (in days): ";
			            		cin >> lengthOfStay;                           // Get new length of stay
			            		
			            		// Update treatment with inpatient details
			            		if (inventory.updateTreatment(patient, oldTreatment, newTreatment, treatmentDate, priority, department, admissionDate, dischargeDate, lengthOfStay)) {
			                    	cout << "\n----- Treatment Edited -----\n" << endl;
			                    	system("pause");
				        			system("CLS");
								}
								else {
									cout << "\n----- Treatment Not Found for patient ID -----\n" << patient->patientID << endl;
									system("pause");
				        			system("CLS");
								}
							}
						} else {
		                    cout << "\n----- Patient ID not found -----\n" << endl;
		                    system("pause");
				        	system("CLS");
		                }
		                break;
                    }
                    default: {
                    	cout << "\n----- Invalid option. Please try again -----\n" << endl;
                        system("pause");
				        system("CLS");
					}
                }
                break;
            }
            case 2: {
        		system("pause");
				system("CLS");
				
				cout << "==== Patient Record ====" << endl;
                // Display all patients in the inventory
				inventory.displayPatients();
				
				system("pause");
				system("CLS");
                
				break;
			}
            case 3: {
            	system("pause");
				system("CLS");
            	
            	int option;
            	// Display the admission and discharge menu options
                displayAdmDisMenu();
                cin >> option;
				
                switch (option) {
                	//Schedule for Patient Admission
                	case 1: {
                		system("pause");
						system("CLS");
						
                		int patientID;
				 		cout <<" ===== List of Patient Required Admission =====" << endl;
				 		
				 		// Sort patients for admission and display
						inventory.linkedInsertionSort();
						inventory.displayAdmissionList();
						
						// Prompt for patient ID to schedule for admission
						cout << "Enter Patient ID to Schedule for Admission: ";
						cin >> patientID;
						
						 // Search for patient by ID in the inventory
						patientRecord* queuePatient = inventory.searchByID(patientID);
						if (queuePatient != NULL) {
							 // Enqueue patient into admission queue
							admissionQueue.enqueue(queuePatient);
							cout << "Schedule admission for Patient ID " << queuePatient->patientID << " - " << queuePatient->name << endl << endl;
							system("pause");
							system("CLS");
						} else {
							cout << "\n---- Patient ID not found -----\n" << endl;
							system("pause");
							system("CLS");
						}
						break;
					}
					
					//Display Patient Admission Queue
					case 2: {
						system("pause");
						system("CLS");
						
						cout << "==== Admission Queue ====" << endl;
						// Display admission queue
						admissionQueue.displayQueue();
						
						system("pause");
						system("CLS");
						break;
					}
					
					//Admit Patient
					case 3: {
						system("pause");
						system("CLS");
						
						cout << "=============" << endl;
		                
		                // Dequeue patient from admission queue (admit patient)
		                admissionQueue.dequeue();
		                cout << "=============" << endl;
		                
		                system("pause");
						system("CLS");
						break;
					}
					
					//Displayed Admitted Patient
					case 4: {
						system("pause");
						system("CLS");
						
						cout << "==== List of Current Admitted Patients ====" << endl;
						// Display all admitted patients
						inventory.displayAdmittedPatients();
						
						system("pause");
						system("CLS");
						break;
					}
					
					//Schedule for Patient Discharge
					case 5: {
						system("pause");
						system("CLS");
						
						int patientID;
				  		
				  		 // Sort admitted patients and display
						inventory.linkedInsertionSort();
						cout << "==== List of Current Admitted Patients ====" << endl;
						inventory.displayAdmittedPatients();
						
						// Prompt for patient ID to schedule for discharge
						cout << "Enter Patient ID to Schedule for Discharge: ";
						cin >> patientID;
						 // Search for patient by ID in the inventory
						patientRecord* queuePatient = inventory.searchByID(patientID);
						if (queuePatient != NULL) {
							// Enqueue patient into discharge queue
							dischargeQueue.enqueue(queuePatient);
							cout << "Schedule discharge for Patient ID " << queuePatient->patientID << " - " << queuePatient->name << endl << endl;
							system("pause");
							system("CLS");
						} else {
							cout << "\n----- Patient ID not found -----\n" << endl;
							system("pause");
							system("CLS");
						}
						break;
					}
					
					//Display Discharged Patient Queue
					case 6: {
						system("pause");
						system("CLS");
						
						cout << "==== Discharged Queue =====" << endl;
						dischargeQueue.displayQueue();
						
						system("pause");
						system("CLS");
						break;
					}
					
					//Discharge Patient
					case 7: {
						system("pause");
						system("CLS");
						
						cout << "=================" << endl;
		                // Dequeue patient from discharge queue (discharge patient)
		                dischargeQueue.dequeue();
		                cout << "=================" << endl;
		                
		                system("pause");
						system("CLS");
						break;
					}
					
					//Displayed Discharged Patient
					case 8: {
						system("pause");
						system("CLS");
						
						cout << "==== List of Discharged Patient ====" << endl;
						// Display all discharged patients
						inventory.displayDischargedPatients();
						
						system("pause");
						system("CLS");
						break;
					}
                	
                	default: {
                		cout << "\n----- Invalid option. Please try again -----\n" << endl;
                		system("pause");
						system("CLS");
					}
				}
				break;
			}
            case 4: {
            	system("pause");
				system("CLS");
            
                int searchOption;
                // Display the search menu options
                displaySearchMenu();
                cin >> searchOption;

                switch (searchOption) {
                	
                	// Search Patient by Name
                    case 1: {
                    	system("pause");
						system("CLS");
						
                        string name;
                        cout << "\n==== Search Patient by Name =====" << endl;
                        cout << "Enter Patient Name: ";
                        cin.ignore();
                        getline(cin, name);                  // Get patient name
                        inventory.searchByName(name);        // Call inventory method to search by name
                        
                        system("pause");
						system("CLS");
                        break;
                    }
                    // Search Patient by ID
                    case 2: {
                    	system("pause");
						system("CLS");
						
                        int patientID;
		                cout << "\n==== Search Patient by ID ====" << endl;
	
		                cout << "Enter Patient ID: ";
		                cin >> patientID;
		                
		                // Search for patient by ID in the inventory
		                patientRecord* patient = inventory.searchByID(patientID);
		                if (patient != NULL) {
		                	// Display patient information and treatments
		                	cout << "+------------+--------------+-------------------------+----------------+----------+---------------------------+----------------+----------------+----------------+\n";
						    cout << "| Patient ID | Name         | Treatment Type          | Treatment Date | Priority | Department                | Admission Date | Discharge Date | Length of Stay |\n";
						    cout << "+------------+--------------+-------------------------+----------------+----------+---------------------------+----------------+----------------+----------------+\n";
		                	
		                    treatmentRecord* tTemp = patient->treatmentHead;
		                    while (tTemp != NULL) {
		                        cout << "| " << setw(11) << left << patient->patientID
					                 << "| " << setw(13) << left << patient->name
					                 << "| " << setw(24) << left << tTemp->treatmentType
					                 << "| " << setw(15) << left << tTemp->treatmentDate
					                 << "| " << setw(9) << left << tTemp->priority
					                 << "| " << setw(26) << left << tTemp->department;
					                 
			                    if (tTemp->InpatientHead->admissionDate != "") {
			                    	InpatientRecord* tTemp2 = tTemp->InpatientHead;
			                    	cout << "| " << setw(15) << left << tTemp2->admissionDate
					                     << "| " << setw(15) << left << tTemp2->dischargeDate
					                     << "| " << setw(15) << left << tTemp2->lengthOfStay;
								} else {
					                cout << "| " << setw(15) << left << "-"
					                     << "| " << setw(15) << left << "-"
					                     << "| " << setw(15) << left << "-";
					            }
					            cout << "|\n";
								tTemp = tTemp->next;
		                    }
		                    cout << "+------------+--------------+-------------------------+----------------+----------+---------------------------+----------------+----------------+----------------+\n";
		                    
		                    system("pause");
							system("CLS");
							
		                } else {
		                    cout << "\n----- Patient ID not found -----\n" << endl;
		                    system("pause");
							system("CLS");
		                }
                        break;
                    }
                    // Search Patient by Treatment
                    case 3: {
                    	system("pause");
						system("CLS");
						
                        string treatment;
                        cout << "\n==== Search Patient by Treatment ====" << endl;
                        
                        cout << "Enter Treatment Type: ";
                        cin.ignore();
                        getline(cin, treatment);                 // Get treatment type
                        inventory.searchByTreatment(treatment);  // Call inventory method to search by treatment
                        
                        system("pause");
						system("CLS");
                        break;
                    }
                    default: {
                    	cout << "\n---- Invalid option. Please try again -----\n" << endl;
                        system("pause");
						system("CLS");
					}
                }
                break;
            }
            case 5: {
            	system("pause");
				system("CLS");
				
                int sortOption;
                // Display the sort menu options
                displaySortMenu();
                cin >> sortOption;

                switch (sortOption) {
                	// Sort by Treatment Date
                    case 1: {
                    	// Call inventory method to sort by treatment date
                        inventory.sortByTreatmentDate(); 
                        cout << "\n----- Patients sorted by Treatment Date -----\n" << endl;
                        
                        system("pause");
						system("CLS");
						break;
					}
                        
                    // Sort by Priority
                    case 2: {
                    	// Call inventory method to sort by priority
                        inventory.sortByPriority();
                        cout << "\n----- Patients sorted by Priority -----\n" << endl;
                        
                        system("pause");
						system("CLS");
						break;
					}
                        
                    // Sort by Length of Stay
                    case 3: {
                    	// Call inventory method to sort by length of stay
                        inventory.sortByLengthOfStay();
                        cout << "\n----- Patients sorted by Length of Stay -----\n" << endl;
                        
                        system("pause");
						system("CLS");
						break;
					}
                        
                    //Sorted by Patient ID (default)
                    case 4: {
                    	inventory.linkedInsertionSort();
                    	cout << "\n----- Patients sorted by Patient ID (default) -----\n" << endl;
                    	
                    	system("pause");
						system("CLS");
						break;
					}
						
                    default: {
                    	cout << "\n----- Invalid option. Please try again -----\n" << endl;
                        system("pause");
						system("CLS");
					}
                }
                break;
            }
            case 6: {
            	system("pause");
				system("CLS");
				
		        cout << "\n================================================= Summary Report ==================================================" << endl;
		        cout << endl;
		        cout << "==== List of Current Admitted Patients ====" << endl;
				// Display all admitted patients
				inventory.displayAdmittedPatients();
				cout << endl << endl;
		        
		        cout << "==== Patients Scheduled for Discharge ====" << endl;
		        // Display discharge queue
				dischargeQueue.displayQueue();
		        cout << endl << endl;
		        
		        cout<< "==== Outpatients' Treatments Sorted by Department  ====" << endl;
		        
		        inventory.sortByDepartment(); // Sort the patients by department before displaying
		        inventory.displayOutPatients();  // Display outpatient treatments
		        
		        cout << endl;
		        system("pause");
				system("CLS");
				break;
			}
			//exit the program
            case 7: {
            	cout << "\n----- Exiting program -----\n" << endl;
                system("pause");
				system("CLS");
				break;
			}
            default: {
            	cout << "\n----- Invalid option. Please choose a valid option -----\n" << endl;
                system("pause");
				system("CLS");
			}
		}
    } while (choice != 7);

    return 0;
}

