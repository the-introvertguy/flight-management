# Flight Management System ✈️

A **Flight Management System** project developed as part of the coursework for **Data Structures (CSE124)** at **Daffodil International University**. This project is instructed by **Professor Dr. Sheak Rashed Haider Noori**, Head of the Department of Computer Science and Engineering.

---

## 📝 Project Overview

The **Flight Management System** is a C-based application designed to manage airport flight operations efficiently . It provides a user-friendly interface for managing flights, including features such as adding flights, displaying all flights, managing seat allocations, delaying or canceling flights, checking flight statuses, viewing statistics and history, and prioritizing flights based on specific criteria.

### Features
- **Add Flight**: Add new flights to the system.
- **Display All Flights**: View a list of all scheduled flights.
- **Depart/Land Flight**: Mark flights as departed or landed.
- **Seat Allocation**: Manage seat assignments for passengers.
- **Delay Flight**: Delay a flight and update its schedule.
- **Cancel Flight**: Remove a flight from the schedule.
- **Check Flight Status**: Check the current status of a flight.
- **View Statistics**: View flight-related statistics.
- **Flight History**: Access the history of past flights.
- **Flight Prioritization**: Assign priority levels to flights (e.g., Medical Emergency, VIP, Regular).

---

## 📚 Course Information

- **Course Title**: Data Structures  
- **Course Code**: CSE124  
- **Semester**: Spring-25  
- **Instructor**: [Dr. Sheak Rashed Haider Noori](https://faculty.daffodilvarsity.edu.bd/profile/cse/rashed-haider-noori.html)  
- **Institution**: Daffodil International University

---

## 👥 Authors

This project was developed by the following team members:

- **[Md Mehedi Hasan Jihad](https://www.github.com/the-introvertguy)**  
- **[Mst Sayma Karina](https://www.github.com/mstsaymakarina)**  
- **[Tanjila Jahan Maisha](https://www.github.com/denizz02)**  
- **[Shishir Kumar Sarkar](https://www.github.com/shishir470)**  

---

## 🛠️ Prerequisites

> [!IMPORTANT]  
> This program is designed with **C**. A C compiler is mandatory to compile and run the code.

To run this project, ensure you have the following installed:
- A C compiler (e.g., `gcc` for Linux/Mac or MinGW for Windows).
- Git (to clone the repository).
- A terminal or command-line interface.
- (Optional) An IDE like Visual Studio Code, CLion, or Code::Blocks for easier development.

---

## 🚀 Getting Started

Follow these steps to clone, compile, and run the project on your local machine.

### 1. Clone the Repository
Clone the repository from GitHub using the following command:

```bash
git clone https://github.com/the-introvertguy/flight-management.git
```

Navigate to the project directory:

```bash
cd flight-management
```

### 2. Compile the Code
Compile the C code using a C compiler. If you're using `gcc`, run the following command:

```bash
gcc main.cpp -o flight_management
```

This will generate an executable file named `flight_management`.

### 3. Run the Program
Execute the compiled program:

- **On Linux/Mac**:
```bash
./flight_management
```

- **On Windows**:
```bash
flight_management.exe
```

### 4. Using the Program
Once the program is running, you'll see the main menu of the Flight Management System:

![Main Menu](https://cdn.discordapp.com/attachments/878331193421099018/1359066654423650455/image.png?ex=67f620f4&is=67f4cf74&hm=31fb4e361aa12649d2eac6a0e905075b5ef3d6ca8e4ccabc3e02ffd8d98647d7&)

- Select an option by entering a number (1-10) and press Enter.
- For example, to prioritize a flight, select option `1` (Add Flight), and you may be prompted to set a priority level:

![Priority Levels](https://media.discordapp.net/attachments/878331193421099018/1359064355546730606/Screenshot_2025-04-08_122556.png?ex=67f61ed0&is=67f4cd50&hm=acfa3a33dc5a0a8bd80085b7cb07e0eb58788865b14774b1a3c7cd3403ba9f1c&=&format=webp&quality=lossless)

Follow the on-screen instructions to interact with the system.

---

## 📸 Screenshots

## Main Menu
The main menu provides access to all the features of the system.

![Main Menu](https://media.discordapp.net/attachments/878331193421099018/1359064355811098655/Screenshot_2025-04-08_122620.png?ex=67f61ed0&is=67f4cd50&hm=4b05f11281cee5fc70da9edcc80e3acd0775e938cfe2b4530f494fd49b7e180f&=&format=webp&quality=lossless)

## Statistics
Shows available seats, average delay percentile, amount of incoming and outgoing fights.

![Statistics](https://media.discordapp.net/attachments/878331193421099018/1359065620414599338/image.png?ex=67f61ffe&is=67f4ce7e&hm=b9f841258226ccde99b6c099444753294310936bf1e58292f928b7dc742419d5&=&format=webp&quality=lossless)

---

## 🧑‍💻 Usage

1. **Add a Flight**: Choose option `1` to add a new flight. You may need to specify details like flight number, destination, and priority.
2. **Display Flights**: Select option `2` to view all flights in the system.
3. **Flight Prioritization**: The system supports a priority-based scheduling mechanism with levels like Medical Emergency (highest priority) to Regular Flight (lowest priority).
4. **Exit the Program**: Choose option `10` to exit the system.

---

## 📜 Code Structure

The project is primarily written in C and uses data structures to manage flight information efficiently. Here's a brief overview of the codebase:

- **`main.cpp`**: The entry point of the program, containing the main menu and user interaction logic.
- **Flight Management Logic**: Implements core functionalities like adding, delaying, and canceling flights.
- **Data Structures**: Likely uses arrays, linked lists, or priority queues to manage flight data and prioritization.

---

## 🤝 Contributing

Contributions are welcome! If you'd like to contribute to this project, please follow these steps:

1. Fork the repository.
2. Create a new branch (`git checkout -b feature/your-feature-name`).
3. Make your changes and commit them (`git commit -m "Add your message"`).
4. Push to the branch (`git push origin feature/your-feature-name`).
5. Open a pull request.

---

## 📧 Contact

For any inquiries or suggestions, feel free to reach out to the authors:

- [**Md Mehedi Hasan Jihad**](https://www.github.com/the-introvertguy)  
- [**Mst Sayma Karina**](https://www.github.com/mstsaymakarina)  
- [**Tanjila Jahan Maisha**](https://www.github.com/denizz02)  
-  [**Shishir Kumar Sarkar**](https://www.github.com/shishir470)  

---

## 🙏 Acknowledgments

- **Professor Dr. Sheak Rashed Haider Noori** for guiding us throughout this project.
- **Daffodil International University** for providing the resources and environment to work on this project.
- Our team for the collaborative effort in building this system.

---

## 📄 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

### Happy Flying! ✈️



