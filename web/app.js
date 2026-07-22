const records = [];
const LOCAL_STORAGE_KEY = 'purchasePatternRecords';

const purchaseForm = document.getElementById('purchaseForm');
const purchaseTableBody = document.getElementById('purchaseTableBody');
const totalRevenueEl = document.getElementById('totalRevenue');
const topProductEl = document.getElementById('topProduct');
const topCustomerEl = document.getElementById('topCustomer');
const avgOrderValueEl = document.getElementById('avgOrderValue');
const uniqueCustomersEl = document.getElementById('uniqueCustomers');
const totalOrdersEl = document.getElementById('totalOrders');
const categorySalesList = document.getElementById('categorySalesList');
const recommendationEl = document.getElementById('recommendation');
const recommendInput = document.getElementById('recommendInput');
const recommendationOutput = document.getElementById('recommendationOutput');
const recommendButton = document.getElementById('recommendButton');
const loadSampleButton = document.getElementById('loadSample');
const loadCsvFileInput = document.getElementById('loadCsvFile');
const loadCsvButton = document.getElementById('loadCsv');
const exportCsvButton = document.getElementById('exportCsv');
const saveLocalButton = document.getElementById('saveLocal');
const loadLocalButton = document.getElementById('loadLocal');
const searchInput = document.getElementById('searchInput');
const recordCountEl = document.getElementById('recordCount');
const submitBtn = document.getElementById('submitBtn');
const cancelEditBtn = document.getElementById('cancelEdit');
const loadCppDataButton = document.getElementById('loadCppData');
const saveCppDataButton = document.getElementById('saveCppData');

let editingIndex = null; // index of record currently being edited, or null
let categoryChart = null;
let productChart = null;

const getFormValue = (id) => document.getElementById(id).value.trim();

// ---------- filtering ----------

const getFilteredRecords = () => {
  const query = (searchInput?.value || '').trim().toLowerCase();
  if (!query) return records.map((record, index) => ({ record, index }));

  return records
    .map((record, index) => ({ record, index }))
    .filter(({ record }) =>
      record.customerName.toLowerCase().includes(query) ||
      record.customerId.toLowerCase().includes(query) ||
      record.productName.toLowerCase().includes(query) ||
      record.category.toLowerCase().includes(query)
    );
};

// ---------- rendering ----------

const renderTable = () => {
  const filtered = getFilteredRecords();
  recordCountEl.textContent = `${filtered.length} record${filtered.length === 1 ? '' : 's'}`;

  purchaseTableBody.innerHTML = filtered
    .map(({ record, index }) => {
      const total = (record.quantity * record.price).toFixed(2);
      return `
      <tr>
        <td>${record.customerName} <span class="muted">(${record.customerId})</span></td>
        <td>${record.productName}</td>
        <td>${record.category}</td>
        <td>${record.quantity}</td>
        <td>Rs. ${record.price.toFixed(2)}</td>
        <td>${record.date}</td>
        <td>Rs. ${total}</td>
        <td class="row-actions">
          <button type="button" class="icon-btn edit-row" data-index="${index}" title="Edit">Edit</button>
          <button type="button" class="icon-btn delete-row" data-index="${index}" title="Delete">Delete</button>
        </td>
      </tr>`;
    })
    .join('');

  purchaseTableBody.querySelectorAll('.edit-row').forEach((btn) => {
    btn.addEventListener('click', () => startEdit(Number(btn.dataset.index)));
  });
  purchaseTableBody.querySelectorAll('.delete-row').forEach((btn) => {
    btn.addEventListener('click', () => deleteRecord(Number(btn.dataset.index)));
  });
};

const calculateTotalRevenue = () => records.reduce((sum, record) => sum + record.price * record.quantity, 0);

const getTopSellingProduct = () => {
  if (!records.length) return '—';

  const totals = records.reduce((map, record) => {
    map[record.productName] = (map[record.productName] || 0) + record.quantity;
    return map;
  }, {});

  return Object.entries(totals).sort((a, b) => b[1] - a[1])[0][0];
};

const getTopProductTotals = () => {
  const totals = records.reduce((map, record) => {
    map[record.productName] = (map[record.productName] || 0) + record.quantity;
    return map;
  }, {});
  return Object.entries(totals).sort((a, b) => b[1] - a[1]).slice(0, 5);
};

const getHighestSpendingCustomer = () => {
  if (!records.length) return '—';

  const totals = records.reduce((map, record) => {
    const key = `${record.customerName} (${record.customerId})`;
    map[key] = (map[key] || 0) + record.price * record.quantity;
    return map;
  }, {});

  return Object.entries(totals).sort((a, b) => b[1] - a[1])[0][0];
};

const getCategorySales = () => {
  const totals = {};

  records.forEach((record) => {
    totals[record.category] = (totals[record.category] || 0) + record.quantity * record.price;
  });

  return Object.entries(totals).sort((a, b) => b[1] - a[1]);
};

const getUniqueCustomerCount = () => new Set(records.map((r) => r.customerId)).size;

const getAverageOrderValue = () => {
  if (!records.length) return 0;
  return calculateTotalRevenue() / records.length;
};

const persistRecords = () => {
  localStorage.setItem(LOCAL_STORAGE_KEY, JSON.stringify(records));
};

const loadRecordsFromStorage = () => {
  const stored = localStorage.getItem(LOCAL_STORAGE_KEY);
  if (!stored) return false;

  try {
    const parsed = JSON.parse(stored);
    if (!Array.isArray(parsed)) return false;

    records.length = 0;
    records.push(...parsed.map((record) => ({
      customerId: String(record.customerId || ''),
      customerName: String(record.customerName || ''),
      productName: String(record.productName || ''),
      category: String(record.category || ''),
      quantity: Number(record.quantity) || 0,
      price: Number(record.price) || 0,
      date: String(record.date || ''),
    })));

    return records.length > 0;
  } catch (error) {
    return false;
  }
};

const parseCsvText = (text) => {
  return text
    .trim()
    .split(/\r?\n/)
    .map((line) => line.trim())
    .filter((line) => line.length)
    .map((line) => line.split(',').map((value) => value.trim()))
    .filter((values) => values.length >= 7)
    .filter((values) => values[0].toLowerCase() !== 'customerid' && values[0].toLowerCase() !== 'customer_id')
    .map((values) => {
      const [customerId, customerName, productName, category, quantity, price, date] = values;
      return {
        customerId,
        customerName,
        productName,
        category,
        quantity: Number(quantity),
        price: Number(price),
        date,
      };
    })
    .filter((record) => record.customerId && record.customerName && record.productName && record.category && !Number.isNaN(record.quantity) && !Number.isNaN(record.price) && record.date);
};

const exportToCsv = () => {
  if (!records.length) {
    recommendationOutput.textContent = 'No records available to export.';
    return;
  }

  const header = 'customerId,customerName,productName,category,quantity,price,date';
  const csvRows = records.map((record) => {
    return [
      record.customerId,
      record.customerName,
      record.productName,
      record.category,
      record.quantity,
      record.price,
      record.date,
    ].join(',');
  });

  const csvContent = [header, ...csvRows].join('\r\n');
  const blob = new Blob([csvContent], { type: 'text/csv;charset=utf-8;' });
  const url = URL.createObjectURL(blob);
  const link = document.createElement('a');
  link.setAttribute('href', url);
  link.setAttribute('download', 'purchase-records.csv');
  link.style.display = 'none';
  document.body.appendChild(link);
  link.click();
  document.body.removeChild(link);
  URL.revokeObjectURL(url);
};

const getDataFileCsvText = () => {
  return records
    .map((record) => [
      record.customerId,
      record.customerName,
      record.productName,
      record.category,
      record.quantity,
      record.price,
      record.date,
    ].join(','))
    .join('\r\n');
};

const loadRecordsFromDataFile = async () => {
  try {
    const response = await fetch('/data.txt');
    if (!response.ok) throw new Error('Unable to fetch data file');

    const text = await response.text();
    const importedRecords = parseCsvText(text);

    if (!importedRecords.length) {
      recommendationOutput.textContent = 'The C++ data file was found but contains no valid records.';
      return;
    }

    records.length = 0;
    records.push(...importedRecords);
    updateApp();
    recommendationOutput.textContent = `Loaded ${importedRecords.length} records from data.txt.`;
  } catch (error) {
    recommendationOutput.textContent = 'Cannot load C++ data file. Start the local server with server.js and ensure data.txt exists.';
  }
};

const saveRecordsToDataFile = async () => {
  if (!records.length) {
    recommendationOutput.textContent = 'No records to save to data.txt.';
    return;
  }

  try {
    const csvText = getDataFileCsvText();
    const response = await fetch('/save-data', {
      method: 'POST',
      headers: { 'Content-Type': 'text/plain;charset=utf-8' },
      body: csvText,
    });

    if (!response.ok) throw new Error('Save failed');
    recommendationOutput.textContent = 'Saved current records to data.txt.';
  } catch (error) {
    recommendationOutput.textContent = 'Cannot save to C++ data file. Start the local server with server.js.';
  }
};

const updateApp = () => {
  renderTable();
  updateInsights();
  updateCharts();
  persistRecords();
};

const getRecommendation = (product) => {
  if (!records.length || !product) return 'Enter a product name to get a recommendation.';

  const customersWhoBought = records
    .filter((record) => record.productName.toLowerCase() === product.toLowerCase())
    .map((record) => record.customerId);

  if (!customersWhoBought.length) return 'No customers bought that product yet.';

  const recommendationCounts = {};

  records.forEach((record) => {
    if (customersWhoBought.includes(record.customerId) && record.productName.toLowerCase() !== product.toLowerCase()) {
      recommendationCounts[record.productName] = (recommendationCounts[record.productName] || 0) + 1;
    }
  });

  const best = Object.entries(recommendationCounts).sort((a, b) => b[1] - a[1])[0];
  return best ? `${best[0]} is often bought together with ${product}.` : `No recommendation found for ${product}.`;
};

const updateInsights = () => {
  totalRevenueEl.textContent = `Rs. ${calculateTotalRevenue().toFixed(2)}`;
  topProductEl.textContent = getTopSellingProduct();
  topCustomerEl.textContent = getHighestSpendingCustomer();
  avgOrderValueEl.textContent = `Rs. ${getAverageOrderValue().toFixed(2)}`;
  uniqueCustomersEl.textContent = getUniqueCustomerCount();
  totalOrdersEl.textContent = records.length;

  categorySalesList.innerHTML = getCategorySales()
    .map(([category, value]) => `<li><span>${category}</span><strong>Rs. ${value.toFixed(2)}</strong></li>`)
    .join('');
};

// ---------- charts ----------

const CHART_COLORS = ['#6bd3ff', '#ff9d66', '#8f9dff', '#7ee9b0', '#ffce6b', '#ff7ea3'];

const updateCharts = () => {
  const categoryData = getCategorySales();
  const productData = getTopProductTotals();

  const categoryCtx = document.getElementById('categoryChart');
  const productCtx = document.getElementById('productChart');
  if (!categoryCtx || !productCtx || typeof Chart === 'undefined') return;

  const categoryConfig = {
    type: 'doughnut',
    data: {
      labels: categoryData.map(([category]) => category),
      datasets: [{
        data: categoryData.map(([, value]) => value),
        backgroundColor: CHART_COLORS,
        borderWidth: 0,
      }],
    },
    options: {
      responsive: true,
      maintainAspectRatio: false,
      plugins: {
        legend: { position: 'bottom', labels: { color: '#f5f8ff', boxWidth: 12, padding: 12 } },
      },
    },
  };

  const productConfig = {
    type: 'bar',
    data: {
      labels: productData.map(([name]) => name),
      datasets: [{
        label: 'Units Sold',
        data: productData.map(([, qty]) => qty),
        backgroundColor: '#6bd3ff',
        borderRadius: 8,
      }],
    },
    options: {
      responsive: true,
      maintainAspectRatio: false,
      plugins: { legend: { display: false } },
      scales: {
        x: { ticks: { color: '#9bb3d6' }, grid: { display: false } },
        y: { ticks: { color: '#9bb3d6' }, grid: { color: 'rgba(255,255,255,0.06)' }, beginAtZero: true },
      },
    },
  };

  if (categoryChart) {
    categoryChart.data = categoryConfig.data;
    categoryChart.update();
  } else {
    categoryChart = new Chart(categoryCtx, categoryConfig);
  }

  if (productChart) {
    productChart.data = productConfig.data;
    productChart.update();
  } else {
    productChart = new Chart(productCtx, productConfig);
  }
};

// ---------- edit / delete ----------

const startEdit = (index) => {
  const record = records[index];
  if (!record) return;

  editingIndex = index;
  document.getElementById('customerId').value = record.customerId;
  document.getElementById('customerName').value = record.customerName;
  document.getElementById('productName').value = record.productName;
  document.getElementById('category').value = record.category;
  document.getElementById('quantity').value = record.quantity;
  document.getElementById('price').value = record.price;
  document.getElementById('date').value = record.date;

  submitBtn.textContent = 'Save Changes';
  cancelEditBtn.classList.remove('hidden');
  purchaseForm.scrollIntoView({ behavior: 'smooth', block: 'center' });
};

const stopEdit = () => {
  editingIndex = null;
  submitBtn.textContent = 'Add Record';
  cancelEditBtn.classList.add('hidden');
  clearForm();
};

const deleteRecord = (index) => {
  if (index < 0 || index >= records.length) return;
  records.splice(index, 1);
  if (editingIndex === index) stopEdit();
  updateApp();
  recommendationOutput.textContent = 'Record deleted.';
};

const clearForm = () => {
  purchaseForm.reset();
};

const showRecommendation = () => {
  const product = recommendInput.value.trim();
  const result = getRecommendation(product);
  recommendationOutput.textContent = result;
  recommendationEl.textContent = result;
};

purchaseForm.addEventListener('submit', (event) => {
  event.preventDefault();

  const purchase = {
    customerId: getFormValue('customerId'),
    customerName: getFormValue('customerName'),
    productName: getFormValue('productName'),
    category: getFormValue('category'),
    quantity: Number(getFormValue('quantity')),
    price: Number(getFormValue('price')),
    date: getFormValue('date'),
  };

  if (editingIndex !== null) {
    records[editingIndex] = purchase;
    recommendationOutput.textContent = 'Record updated.';
    stopEdit();
  } else {
    records.push(purchase);
    showRecommendation();
    clearForm();
  }

  updateApp();
});

cancelEditBtn.addEventListener('click', stopEdit);

searchInput.addEventListener('input', renderTable);

recommendButton.addEventListener('click', showRecommendation);
loadSampleButton.addEventListener('click', () => {
  const sampleData = [
    { customerId: 'C001', customerName: 'Ayesha Khan', productName: 'Milk', category: 'Dairy', quantity: 3, price: 70, date: '2026-04-01' },
    { customerId: 'C002', customerName: 'Rahul Patel', productName: 'Tea Biscuits', category: 'Snacks', quantity: 5, price: 35, date: '2026-04-02' },
    { customerId: 'C001', customerName: 'Ayesha Khan', productName: 'Paneer', category: 'Dairy', quantity: 2, price: 240, date: '2026-04-02' },
    { customerId: 'C003', customerName: 'Sonal Mehta', productName: 'Shampoo', category: 'PersonalCare', quantity: 1, price: 180, date: '2026-04-03' },
    { customerId: 'C004', customerName: 'Ravi Sharma', productName: 'Notebook', category: 'Stationery', quantity: 4, price: 55, date: '2026-04-04' },
    { customerId: 'C002', customerName: 'Rahul Patel', productName: 'Chocolates', category: 'Snacks', quantity: 2, price: 120, date: '2026-04-05' },
    { customerId: 'C005', customerName: 'Meera Singh', productName: 'Juice', category: 'Beverage', quantity: 3, price: 95, date: '2026-04-05' },
    { customerId: 'C001', customerName: 'Ayesha Khan', productName: 'Tea Biscuits', category: 'Snacks', quantity: 1, price: 35, date: '2026-04-06' },
  ];

  records.length = 0;
  records.push(...sampleData);
  updateApp();
  recommendationOutput.textContent = 'Sample data loaded. Ask for a recommendation!';
});

loadCsvButton.addEventListener('click', () => {
  const file = loadCsvFileInput.files[0];
  if (!file) {
    recommendationOutput.textContent = 'Choose a CSV file before importing.';
    return;
  }

  const reader = new FileReader();
  reader.onload = () => {
    const importedRecords = parseCsvText(reader.result);
    if (!importedRecords.length) {
      recommendationOutput.textContent = 'No valid records found in CSV file.';
      return;
    }

    records.length = 0;
    records.push(...importedRecords);
    updateApp();
    recommendationOutput.textContent = `Imported ${importedRecords.length} records from CSV.`;
  };
  reader.readAsText(file);
});

exportCsvButton.addEventListener('click', exportToCsv);

saveLocalButton.addEventListener('click', () => {
  persistRecords();
  recommendationOutput.textContent = 'Records saved to browser storage.';
});

loadLocalButton.addEventListener('click', () => {
  const loaded = loadRecordsFromStorage();
  if (loaded) {
    updateApp();
    recommendationOutput.textContent = 'Loaded records from browser storage.';
  } else {
    recommendationOutput.textContent = 'No saved records found in local storage.';
  }
});

loadCppDataButton.addEventListener('click', loadRecordsFromDataFile);
saveCppDataButton.addEventListener('click', saveRecordsToDataFile);

window.addEventListener('DOMContentLoaded', () => {
  const loaded = loadRecordsFromStorage();
  if (loaded) {
    recommendationOutput.textContent = 'Saved records restored. Ask for a recommendation!';
  } else {
    recommendationOutput.textContent = 'Load sample data or add a purchase to begin.';
  }
  updateApp();
});
